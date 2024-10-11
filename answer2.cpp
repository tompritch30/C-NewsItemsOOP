#include <string>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;


class Fruit {
protected:
    // enum FruitType{RED_APPLE, MIXED_APPLE, PEAR};
    int weight;
    int bestBeforeDays;
    string variety;
    string name;
    // FruitType type;

public:
    Fruit(int _w, int _d, string _variety) : weight(_w), bestBeforeDays(_d), variety(std::move(_variety)){}

    bool fitForSale() const{
        return bestBeforeDays >= 3;
    }

    virtual bool isPremiumClass() const = 0;


    virtual string getAdditionalInformation() const = 0;

    string getInformation() {
        return to_string(weight) + "g , " + to_string(bestBeforeDays) + " days before expires, of variety: " + variety +  " " + getAdditionalInformation();
    }
};

class Apple : public Fruit {
protected:
    double redSurfaceArea;
public:
    Apple(int _w, int _d, string _variety, double _surface) : Fruit(_w, _d, std::move(_variety)), redSurfaceArea(_surface) {}

    virtual bool hasMinRedSurface() const = 0;

    bool isPremiumClass() const override{
        return (weight > 90 && hasMinRedSurface());
    }

    string getAdditionalInformation() const override {
        return to_string(redSurfaceArea) + " surface area.";
    }

};

class RedApple : public Apple {
public:
    RedApple(int _w, int _d, string _variety, double _surface) : Apple(_w, _d, std::move(_variety), _surface) {}

    bool hasMinRedSurface() const override {
        return (redSurfaceArea >= 75);
    }

};

class MixedApple : public Apple {
public:
    MixedApple(int _w, int _d, string _variety, double _surface) : Apple(_w, _d, std::move(_variety), _surface) {}

    bool hasMinRedSurface() const override {
        return (redSurfaceArea >= 50);
    }

};

class Pear : public Fruit {
protected:
    bool stalkIntact;

public:
    Pear(int _w, int _d, string _variety, bool _stalkIntact) : Fruit(_w, _d, std::move(_variety)), stalkIntact(_stalkIntact) {}

    bool isPremiumClass() const override{
        return (weight > 130 && stalkIntact);
    }

    string getAdditionalInformation() const override {
        return stalkIntact ? "with stalk intact" : "without stalk intact";
    }
};

template<typename T>
class FruitBasket {

private:
    // int maxFruits;
    int currentCapacity;
    list<T*> fruitsInBasket;
    int maxFruits;

public:
    explicit FruitBasket(int _maxFruits) : maxFruits(_maxFruits), currentCapacity(0) {}

    void printBasketContents(){
        if (!fruitsInBasket.empty()) {
            for (Fruit* fruit : fruitsInBasket) {
                cout << fruit->getInformation() << endl;
            }
        }
    }

    bool addItemToBasket(T * fruit) {
        if (currentCapacity < maxFruits) {
            fruitsInBasket.push_back(fruit);
            currentCapacity++;
            return true;
        }
        return false;
    }

    T * getAnItem(){
        if (!fruitsInBasket.empty()) {
            // fruitsInBasket.pop_back();
            currentCapacity--;
            Fruit * fruit = fruitsInBasket.back();
            fruitsInBasket.pop_back();
            return fruit;
        }
        return nullptr;
    }

    bool isEmpty(){
        return fruitsInBasket.size() == 0;
    }
};

//
//int main() {
//    FruitBasket<Fruit> basket = FruitBasket<Fruit>(30);
//    RedApple redApple = RedApple(95, 7, "Gala", 80);
//    MixedApple mixedApple = MixedApple(80, 8, "Cox", 60);
//    Pear pear = Pear(140, 2, "ambrosia", true);
//
//    basket.addItemToBasket(&redApple);
//    basket.addItemToBasket(&mixedApple);
//    basket.addItemToBasket(&pear);
//
//    basket.printBasketContents();
//
//    FruitBasket<Fruit> smallBasket = FruitBasket<Fruit>(10);
//
//
//   while(!basket.isEmpty()){
//        Fruit* fruit = basket.getAnItem();
//        if (fruit->fitForSale() && fruit->isPremiumClass()) {
//            cout << "Added a premium fruit fit for sale!" << endl;
//            smallBasket.addItemToBasket(fruit);
//        }
//    }
//    cout << "State of baskets after. \nLarge basket: " << endl;
//    basket.printBasketContents();
//    cout << "Small basket : " << endl;
//    smallBasket.printBasketContents();
//
//    // cout << "test";
//    return 0;
//}