#include <iostream>
#include <map>
#include <set>
#include <unordered_map>//ассоциативное хранилище «ключ-значение» без сортировки, с быстрым доступом
#include <unordered_set>//хранит уникальные элементы без сортировки, используя хеш-таблицы для быстрого доступа
#include <string>
#include <locale.h>

using namespace std;


class Deal
{
public:
    string id;
    string type; 
    string property_address;
    double price;
    string buyer;
    string seller;

    Deal() = default;
    Deal(string id_, string type_, string property_address_, double price_, string buyer_, string seller_)
        : id(id_), type(type_), property_address(property_address_), price(price_), buyer(buyer_), seller(seller_) {}

    void display() const
    {
        std::cout << "ID: " << id << ", Тип: " << type
            << ", Адрес: " << property_address
            << ", Цена: " << price
            << ", Покупатель: " << buyer
            << ", Продавец: " << seller << std::endl;
    }

    bool operator<(const Deal& other) const// перегружаем тк надодля сортировки эл map
    {
        return id < other.id;
    }

    bool operator==(const Deal& other) const// перегружаем для unordered_set
    {
        return id == other.id;
    }
};

// Добавляем хеш-функцию для класса Deal, чтобы его можно было использовать в unordered_set и unordered_map
namespace std 
{
    template <>
    struct hash<Deal>//специализацию структуры hash для типа Deal
    {
        size_t operator()(const Deal& deal) const//будет вызываться для получения хеш-значения объекта типа Deal
        {
            return hash<string>()(deal.id); //меняет хеш на айди или на оборот 
        }
    };
}


void showMenu()
{
    std::cout << "1. Добавить сделку\n";
    std::cout << "2. Удалить сделку\n";
    std::cout << "3. Редактировать сделку\n";
    std::cout << "4. Поиск сделки\n";
    std::cout << "5. Вывести все сделки\n";
    std::cout << "6. Выход\n";
}

int main()
{
    map<string, Deal> deals_map;// контейнер для хранения уникальных сделок с сортировкой по ID
    unordered_map<string, Deal> deals_umap;// контейнер для быстрого доступа к сделкам без сортировки
    set<Deal> deals_set;// контейнер  хранения уникальных сделок с сортировкой
    unordered_set<Deal> deals_uset;// контейнер  для уникальных сделок без сортировки

    int choice;

    do
    {
        setlocale(LC_ALL, "Rus");
        showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string id, type, address, buyer, seller;
            double price;
            cout << "Введите ID сделки: ";
            cin >> id;
            cout << "Введите тип сделки (Покупка/Продажа): ";
            cin >> type;
            cout << "Введите адрес недвижимости: ";
            cin.ignore(); // чтобы избежать проблем с вводом строки после числового значения
            getline(cin, address);
            cout << "Введите цену: ";
            cin >> price;
            cout << "Введите покупателя: ";
            cin.ignore();
            getline(cin, buyer);
            cout << "Введите продавца: ";
            getline(cin, seller);

            Deal new_deal(id, type, address, price, buyer, seller);

            // Добавляем сделку в map и unordered_map
            deals_map[id] = new_deal;// тут ключ значение 
            deals_umap[id] = new_deal;
            // Добавляем сделку в set и unordered_set
            deals_set.insert(new_deal);
            deals_uset.insert(new_deal);//тут уникальный элемент 

            cout << "Сделка успешно добавлена!\n";
            break;
        }
        case 2:
        {
            string id;
            cout << "Введите ID сделки для удаления: ";
            cin >> id;

            // Удаляем сделку из всех контейнеров
            deals_map.erase(id);
            deals_umap.erase(id);
            
            Deal temp_deal(id, "", "", 0, "", ""); 
            deals_set.erase(temp_deal);// используется перпегрузка
            deals_uset.erase(temp_deal);//испольбзуется хеш-функция

            cout << "Сделка успешно удалена!\n";
            break;
        }
        case 3:
        {
            string id;
            cout << "Введите ID сделки для редактирования: ";
            cin >> id;
            if (deals_map.find(id) != deals_map.end())
            {
                
                string type, address, buyer, seller;
                double price;
                cout << "Введите новый тип сделки (Покупка/Продажа): ";
                cin >> type;
                cout << "Введите новый адрес недвижимости: ";
                cin.ignore();
                getline(cin, address);
                cout << "Введите новую цену: ";
                cin >> price;
                cout << "Введите нового покупателя: ";
                cin.ignore();
                getline(cin, buyer);
                cout << "Введите нового продавца: ";
                getline(cin, seller);

                Deal edited_deal(id, type, address, price, buyer, seller);

                
                deals_map[id] = edited_deal;
                deals_umap[id] = edited_deal;
               
                deals_set.erase(Deal(id, "", "", 0, "", ""));  
                
                deals_set.insert(edited_deal);  
               
                deals_uset.erase(Deal(id, "", "", 0, "", "")); 
                
                deals_uset.insert(edited_deal); 

                cout << "Сделка успешно отредактирована!\n";
            }
            else
            {
                cout << "Сделка не найдена!\n";
            }
            break;
        }
        case 4:
        {
            string id;
            cout << "Введите ID сделки для поиска: ";
            cin >> id;
            if (deals_map.find(id) != deals_map.end())
            {
                deals_map[id].display();
            }
            else
            {
                cout << "Сделка не найдена!\n";
            }
            break;
        }
        case 5:
        {
            cout << "Содержимое map:\n";
            for (const auto& pair : deals_map)
            {
                pair.second.display();
            }

            cout << "Содержимое unordered_map:\n";
            for (const auto& pair : deals_umap)
            {
                pair.second.display();
            }

            cout << "Содержимое set:\n";
            for (const auto& deal : deals_set)
            {
                deal.display();
            }

            cout << "Содержимое unordered_set:\n";
            for (const auto& deal : deals_uset)
            {
                deal.display();
            }
            break;
        }
        case 6:
            cout << "Выход.\n";
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова.\n";
        }
    } while (choice != 6);

    return 0;
}
