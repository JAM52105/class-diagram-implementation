#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Product {
public:
    int productID;
    string name;
    double price;
    int stockQuantity;

    Product(int id, string n, double p, int stock) : productID(id), name(n), price(p), stockQuantity(stock) {}
};

class ShoppingCart {
public:
    vector<pair<Product*, int>> cart;
    
    void addProduct(Product* product, int quantity) {
        if (quantity <= 0) {
            cout << "Invalid quantity!" << endl;
            return;
        }

        if (quantity > product->stockQuantity) {
            cout << "Not enough stock available!" << endl;
            return;
        }

        for (auto &item : cart) {
            if (item.first->productID == product->productID) {
                if (item.second + quantity > product->stockQuantity) {
                    cout << "Not enough stock available!" << endl;
                    return;
                }
                item.second += quantity;
                product->stockQuantity -= quantity;
                cout << "Product added successfully!" << endl;
                return;
            }
        }

        cart.push_back({product, quantity});
        product->stockQuantity -= quantity;
        cout << "Product added successfully!" << endl;
    }

    void viewCart() {
        if (cart.empty()) {
            cout << "Your shopping cart is empty!" << endl;
            return;
        }
        
        cout << "\nShopping Cart:" << endl;
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Qty" << endl;
        for (auto &item : cart) {
            cout << left << setw(10) << item.first->productID << setw(20) << item.first->name 
                 << setw(10) << item.first->price << setw(10) << item.second << endl;
        }
    }

    double calculateTotal() {
        double total = 0;
        for (auto &item : cart) {
            total += item.first->price * item.second;
        }
        return total;
    }

    void clearCart() {
        cart.clear();
    }
};

class Order {
public:
    static int orderCounter;
    int orderID;
    ShoppingCart cart;
    double totalAmount;

    Order(ShoppingCart c) : cart(c) {
        orderID = ++orderCounter;
        totalAmount = cart.calculateTotal();
    }

    void checkout() {
        cout << "\nCheckout Summary:" << endl;
        cart.viewCart();
        cout << "Total Price: $" << fixed << setprecision(2) << totalAmount << endl;
        cout << "You have successfully checked out the products!" << endl;
    }
};
int Order::orderCounter = 0;

class OrderHistory {
public:
    vector<Order> orders;
    
    void addOrder(Order order) {
        orders.push_back(order);
    }

    void viewOrders() {
        if (orders.empty()) {
            cout << "No orders found!" << endl;
            return;
        }
        for (auto &order : orders) {
            cout << "\nOrder ID: " << order.orderID << endl;
            cout << "Total Amount: " << fixed << setprecision(2) << order.totalAmount << endl;
            cout << "Order Details:" << endl;
            cout << left << setw(10) << "Product ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Quantity" << endl;
            for (auto &item : order.cart.cart) {
                cout << left << setw(10) << item.first->productID << setw(20) << item.first->name 
                     << setw(10) << item.first->price << setw(10) << item.second << endl;
            }
        }
    }
};

void displayProducts(vector<Product> &products) {
    cout << "\nAvailable Products:" << endl;
    cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Price" << setw(10) << "Stock" << endl;
    for (auto &product : products) {
        cout << left << setw(10) << product.productID << setw(20) << product.name 
             << setw(10) << product.price << setw(10) << product.stockQuantity << endl;
    }
}

int main() {
    vector<Product> products = {
        {1, "Laptop", 999.99, 10},
        {2, "Smartphone", 599.99, 15},
        {3, "Headphones", 199.99, 20},
        {4, "Smartwatch", 249.99, 12}
    };
    ShoppingCart cart;
    OrderHistory orderHistory;
    int choice;

    while (true) {
        cout << "\nMenu:" << endl;
        cout << "1. View Products & Buy" << endl;
        cout << "2. View Shopping Cart" << endl;
        cout << "3. View Orders" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            displayProducts(products);
            while (true) {
                int productID, quantity;
                cout << "Enter the ID of the product you want to add to the shopping cart (0 to go back): ";
                cin >> productID;
                if (productID == 0) break;
                
                bool found = false;
                for (auto &product : products) {
                    if (product.productID == productID) {
                        cout << "Enter quantity: ";
                        cin >> quantity;
                        if (cin.fail()) { // Validate non-numeric input
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid input! Please enter a valid quantity." << endl;
                            continue;
                        }
                        cart.addProduct(&product, quantity);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Invalid Product ID!" << endl;
            }

        } else if (choice == 2) {
            cart.viewCart();
            if (!cart.cart.empty()) {
                char checkoutChoice;
                cout << "Do you want to check out all the products? (Y/N): ";
                cin >> checkoutChoice;
                if (checkoutChoice == 'Y' || checkoutChoice == 'y') {
                    Order order(cart);
                    order.checkout();
                    orderHistory.addOrder(order);
                    cart.clearCart();
                }
            }

        } else if (choice == 3) {
            orderHistory.viewOrders();

        } else if (choice == 4) {
            cout << "Exiting program. Thank you!" << endl;
            break;
        } else {
            cout << "Invalid choice! Please enter a valid option." << endl;
        }
    }
    return 0;
}
