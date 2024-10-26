#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

class Interazione {
public:
    std::string data;
    std::string dettagli;

    Interazione(const std::string& data, const std::string& dettagli)
        : data(data), dettagli(dettagli) {}
};

class Cliente {
public:
    std::string nome;
    std::string cognome;
    std::string email;
    std::vector<Interazione> interazione;

    Cliente(const std::string& nome, const std::string& cognome, const std::string& email)
        : nome(nome), cognome(cognome), email(email) {}
};

class CRM {
private:
    std::vector<Cliente> clienti;

    void salva(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& cliente : clienti) {
            file << cliente.nome << "," << cliente.cognome << "," << cliente.email << "\n";
            for (const auto& interaction : cliente.interazione) {
                file << interaction.data << "," << interaction.dettagli << "\n";
            }
        }
        file.close();
    }

    void carica(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        Cliente* clienteCorrente = nullptr;

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string nome, cognome, email, data, dettagli;
            
            if (std::count(line.begin(), line.end(), ',') == 2) {
                std::getline(ss, nome, ',');
                std::getline(ss, cognome, ',');
                std::getline(ss, email, ',');
                clienti.push_back(Cliente(nome, cognome, email));
                clienteCorrente = &clienti.back();
            } else if (std::count(line.begin(), line.end(), ',') == 1 && clienteCorrente) {
                std::getline(ss, data, ',');
                std::getline(ss, dettagli, ',');
                clienteCorrente->interazione.push_back(Interazione(data, dettagli));
            }
        }
        file.close();
    }

public:
    CRM() {
        carica("data.csv");
    }

    ~CRM() {
        salva("data.csv");
    }

    void aggiungiCliente(const std::string& nome, const std::string& cognome, const std::string& email) {
            clienti.push_back(Cliente(nome, cognome, email));
    }

    void visualizzaClienti() {
        for (const auto& cliente : clienti) {
            std::cout << "Cliente: " << cliente.nome << " " << cliente.cognome << ", Email: " << cliente.email << "\n";
        }
    }

    void modificaCliente(const std::string& nome, const std::string& cognome, const std::string& newEmail) {
        for (auto& cliente : clienti) {
            if (cliente.nome == nome && cliente.cognome == cognome) {
                cliente.email = newEmail;
                return;
            }
        }
    }

    void cancellaCliente(const std::string& nome, const std::string& cognome) {
        clienti.erase(std::remove_if(clienti.begin(), clienti.end(),
            [&](const Cliente& cliente) {
                return cliente.nome == nome && cliente.cognome == cognome;
            }), clienti.end());
    }

    Cliente* ricercaCliente(const std::string& nome, const std::string& cognome) {
        for (auto& cliente : clienti) {
            if (cliente.nome == nome && cliente.cognome == cognome) {
                return &cliente;
            }
        }
        return nullptr;
    }

    void aggiungiInterazione(const std::string& nome, const std::string& cognome, const std::string& data, const std::string& dettagli) {
        Cliente* cliente = ricercaCliente(nome, cognome);
        if (cliente) {
            cliente->interazione.push_back(Interazione(data, dettagli));
        }
    }

    void visualizzaInterazione(const std::string& nome, const std::string& cognome) {
        Cliente* cliente = ricercaCliente(nome, cognome);
        if (cliente) {
            for (const auto& interaction : cliente->interazione) {
                std::cout << "Date: " << interaction.data << ", Details: " << interaction.dettagli << "\n";
            }
        }
    }
};

void mostraMenu() {
    std::cout << "__________Menu__________\n";
    std::cout << "1. Aggiungi Cliente\n";
    std::cout << "2. Visualizza Clienti\n";
    std::cout << "3. Modifica Cliente\n";
    std::cout << "4. Elimina Cliente\n";
    std::cout << "5. Cerca Cliente\n";
    std::cout << "6. Aggiungi Interazione\n";
    std::cout << "7. Visualizza Interazioni\n";
    std::cout << "8. Salva ed Esci\n";
    std::cout << "--------------------------\n";
}

int main() {
    CRM crm;
    int opzioneScelta;
    std::string nome, cognome, email, data, dettagli;

    while (true) {
        mostraMenu();
        std::cout << "Scegli un'opzione indicando il numero corrispondente: ";
        std::cin >> opzioneScelta;
        switch (opzioneScelta) {
            case 1:
                std::cout << "Inserisci nome, cognome ed email del cliente (separati da spazio): ";
                std::cin >> nome >> cognome >> email;

                crm.aggiungiCliente(nome, cognome, email);
                std::cout << "Il cliente" << nome << " " << cognome << "è stato aggiunto " << "\n";
                break;
                
            case 2:
                std::cout << "Lista clienti "<< "\n";
                crm.visualizzaClienti();
                break;

            case 3:
                std::cout << "Inserisci nome, cognome del cliente da modificare: ";
                std::cin >> nome >> cognome;
                if (crm.ricercaCliente(nome, cognome)) {


                    std::cout << "Inserisci Nuova Email: ";
                    std::cin >> email;
                    crm.modificaCliente(nome, cognome, email);
                
                }else {
                    std::cout << "Cliente non trovato.\n";
                }
                break;
            case 4:
                std::cout << "Inserisci Nome e Cognome del cliente da eliminare: ";
                std::cin >> nome >> cognome;
                
                crm.cancellaCliente(nome, cognome);
                break;
            case 5:
                std::cout << "Inserisci Nome e Cognome del cliente da cercare: ";
                std::cin >> nome >> cognome;
                if (crm.ricercaCliente(nome, cognome)) {
                    std::cout << "Cliente trovato.\n";
                } else {
                    std::cout << "Cliente non trovato.\n";
                }
                break;
            case 6:
                std::cout << "Inserisci Nome del cliente: ";
                std::cin >> nome;
                std::cout << "Inserisci Cognome del cliente: ";
                std::cin >> cognome;
                std::cout << "Inserisci Data (YYYY-MM-DD): ";
                std::cin >> data;
                std::cout << "Inserisci Dettagli dell'interazione: ";
                std::cin.ignore();
                std::getline(std::cin, dettagli);
                crm.aggiungiInterazione(nome, cognome, data, dettagli);
                break;
            case 7:
                std::cout << "Inserisci Nome del cliente: ";
                std::cin >> nome;
                std::cout << "Inserisci Cognome del cliente: ";
                std::cin >> cognome;
                crm.visualizzaInterazione(nome, cognome);
                break;
            case 8:
                return 0;
            default:
                std::cout << "Opzione non valida.\n";
        }
    }

    return 0;
}
