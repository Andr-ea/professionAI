import json

products = []
sales = []

def load_data():
    """
    Load data from the 'vegan_store.json' file and update the 'products' and 'sales' global lists.
    If the file is not found, an empty list is returned.
    """
    try:
        with open("vegan_store.json", "r") as file:
            data = json.load(file)
            global products, sales
            products = data['products']
            sales = data['sales']
    except FileNotFoundError:
        return []

def save_data():
    """
    Save the current state of 'products' and 'sales' to the 'vegan_store.json' file.
    """
    data = {'products': products, 'sales': sales}
    with open('vegan_store.json', 'w') as file:
        json.dump(data, file)

#Store initialization
load_data()

def help():
    """
    Display a list of available commands.
    """
    print("\nComandi disponibili:")
    print("- aggiungi: Aggiungi un prodotto al magazzino")
    print("- elenca. Elenca i prodotti in magazzino")
    print("- vendita. Registra una vendita effettuata")
    print("- profitti. Mostra i profitti totali")
    print("- aiuto. Mostra i possibili comandi")
    print("- chiudi. Esci dal programma")

def read_number(args):
    """
    Prompt the user for input and convert it to a floating-point number.
    """
    while True:
        try:
            value = float(input(args))
            return value
        except ValueError:
            print("Inserisci un valore numerico valido.")

def add_product():
    """
    Add a product if it is not present, otherwise update the quantity
    """
    try:
        name = input("Nome del prodotto: ").strip()

        while True:
            try:
                amount = (read_number("Quantità: "))
                if amount > 0 and amount.is_integer():
                    break
                else:
                    print("La Quantità deve essere un numero intero positivo")
            except ValueError:
                print("Valore non valido!")

        #Check if the product is already in the store
        existing_product = next((product for product in products if product['name'] == name), None)

        if existing_product:
            existing_product['amount'] += amount
            print(f"La quantità di {name} è stata aggiornata a {existing_product['amount']}.")
            save_data()
        else:
            purchase_price = read_number("Prezzo di acquisto: ")
            selling_price = read_number("Prezzo di vendita: ")

            if ( purchase_price < 0 or selling_price < 0):
                raise ValueError("I valori non possono essere negativi")

            product = {
                'name': name,
                'amount': amount,
                'purchase_price': purchase_price,
                'selling_price': selling_price
            }
            products.append(product)

        save_data()
        print(f"AGGIUNTO: {amount} X {name}")

    except ValueError as e:
        print(f"Error: {e}")

def product_list():
    """
    Display a list of products with quantities and prices.
    """
    print("\nProdotto | Quantità | Prezzo:")
    for product in products:
        print(f"{product['name']} | {product['amount']} | €{product['selling_price']}")

def record_sale():
    """
    Record a sale updating sales and product quantities.
    """
    total = 0
    while True:
        product_name = input("Nome del prodotto venduto: ")
        existing_product = next((product for product in products if product['name'] == product_name), None)
        if (existing_product):
            sold_amount = int(input("Inserisci la quantità venduta: "))
            if sold_amount <= 0:
                raise ValueError("La quantità venduta deve essere maggiore di zero")

            if existing_product['amount'] >= sold_amount:
                sale = {
                    "name": existing_product['name'],
                    "sold_amount": sold_amount,
                    "selling_price": existing_product['selling_price'],
                    "purchase_price": existing_product['purchase_price']
                }
                sales.append(sale)
                existing_product['amount'] -= sold_amount
                if existing_product['amount'] <= 0:
                    products.remove(existing_product)

                total += sale['selling_price'] * sale['sold_amount']
                print(f"Vendita registrata!")
                save_data()
            else:
                print("Quantità non disponibile.")

        else:
            print("Prodotto non disponibile.")

        choise = input("Vendere un altro prodotto? (si/no):")

        if choise.lower() != 'si':
            if total > 0:
                print("Totale: €", round(total,2))
            break

def calculate_gross_profit(sales):
    """
    Calculate the gross profit based on recorded sales.
    """
    return sum(sale['selling_price'] * sale['sold_amount'] for sale in sales)

def calculate_net_profit(sales):
    """
    Calculate the net profit, taking into account purchase costs.
    """
    purchase_cost = sum(sale['purchase_price'] * sale['sold_amount'] for sale in sales )
    gross_profit = calculate_gross_profit(sales)
    net_profit = gross_profit - purchase_cost
    return net_profit

def calculate_profits():
    """
    Calculate and display both gross and net profits.
    """
    gross_profit = calculate_gross_profit(sales)
    net_profit = calculate_net_profit(sales)

    print(f"\nProfitto Lordo: {round(gross_profit,2)}")
    print(f"Profitto Netto: {round(net_profit,2)}")

while True:
    command = input("Inserisci un comando: ")

    if command == 'aggiungi':
        add_product()
    elif command == 'elenca':
        product_list()
    elif command == 'vendita':
        record_sale()
    elif command == 'profitti':
        calculate_profits()
    elif command == 'aiuto':
        help()
    elif command == 'chiudi':
        save_data()
        print("Bye bye")
        break
    else:
        print("Comando non valido! \nI comandi disponibili sono i seguenti:")
        help()
