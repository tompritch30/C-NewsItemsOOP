#include <string>
#include <utility>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

class NewsItem;

class Source;

class Category {
private:
    string categoryType;
    Category *parentCategory;
    static list<Category*> categories;

    explicit Category(string type, Category* _parentPtr=nullptr) : categoryType(std::move(type)), parentCategory(_parentPtr) {}

    static map<Category*, int> CategoryViewCount;

public:
    void setParentCategory(Category *_parentCategory);

    static Category* makeOrGetCategory(const string& name, Category* parentPtr=nullptr){
        if (!categories.empty()){
            for(Category* category: categories){
                if(category->getName() == name){
                    return category;
                }
            }
        }
        return new Category(name, parentPtr);
    }

    static void incrementCategoryCount(Category* category){
        CategoryViewCount[category] += 1;
    }

    static Category* getHighestCategory(){
        int max = INT_MIN;
        Category* highestCategory = nullptr;
        for (pair<Category *const, int> category: CategoryViewCount){
            if (category.second > max){
                max = category.second;
                highestCategory = category.first;
            }
        }
        return highestCategory;
    }

    std::string getName(){
        return categoryType;
    }


};

class WebPage {
private:
    list<NewsItem *> newsItems;
    map<Category, list<NewsItem *>> groupedNewsItems;
    string name;

public:
    explicit WebPage(string _name) : name(std::move(_name)) {}

    void addNewsItem(NewsItem *item) {
        newsItems.push_back(item);
    }

};


class Source {
private:
    list<NewsItem *> newsItems;
    string uniqueUrl;
    static list<Source*> listOfSources;

    explicit Source(string _uniqueUrl) : uniqueUrl(std::move(_uniqueUrl)) {
//        listOfSources.push_back(this);
    }

public:
    static Source* makeOrGetASource(const string& url){
        if (!listOfSources.empty()) {
            for (Source *source: listOfSources) {
                if (source->getUrl() == url) {
                    cout << "found the source!" << endl;
                    return source;
                }
            }
        }
        Source* newSource = new Source(url);
        cout << "made a new source!" << endl;
        listOfSources.push_back(newSource);
        return newSource;
    }

    string getUrl(){
        return uniqueUrl;
    }

    void addNewsItem(NewsItem *item);

    string getContentAndView(const string& basicString);

    ~Source() {
      for (NewsItem* item: newsItems){
          delete item;
      }
      for (Source * src: listOfSources){
          delete src;
      }
      listOfSources.clear();
      newsItems.clear();
    }
};

class NewsItem {
private:
    string title;
    string content;
    string author;

    long int viewCount;
    Source* source;
    Category category;

public:
    NewsItem(const string _title, string _content, string _author, Source* _source, Category &_category)
            : title(_title), content(std::move(_content)), author(std::move(_author)), source(_source),
              viewCount(0),
              category(_category) {
        // content = "hello there";
        // cout << "made news item with the source " << source->getUrl();
    }

    std::string getDetails() {
        return title + ", " + author;
    }

    std::string getContentIfSameTitle(const string& newTitle){
        // return content;
        // cout << "the title input was " << newTitle << " and my title is " << title << endl;
        if (title == newTitle){
            viewCount++;
            Category::incrementCategoryCount(&category);
            return content;
        }
        return "";
    }
};

void Source::addNewsItem(NewsItem *item) {
    std::cout << "Added a news item: " << item->getDetails() << " to the source: " << uniqueUrl << std::endl;
    newsItems.push_back(item);
}

list<Source*> Source::listOfSources = {};

string Source::getContentAndView(const string& basicString) {
    int count = 0;
    for (NewsItem* item: newsItems){

        // cout << ++count << endl;
        // cout << "details of item currently considering" << item->getDetails() << endl;
        string content = item->getContentIfSameTitle(basicString);
        //string content = "";
        // cout << "here is the content returned from the item" << endl;
        if (!content.empty()){
            return content;
        }
    }
    return "+++";
}

void Category::setParentCategory(Category *_parentCategory) {
    parentCategory = _parentCategory;
}

void insertNewsItem(string title, string content, string author, string categoryName, const string& sourceWebpage) {
    // WebPage webpage(sourceWebpage);
    Source* source = Source::makeOrGetASource(sourceWebpage);
    Category* category = Category::makeOrGetCategory(categoryName);
    NewsItem* newsItem = new NewsItem(std::move(title), std::move(content), std::move(author), source, *category);
    source->addNewsItem(newsItem);
}

string getContent(const string& sourceWebpage, const string& title) {
    Source * source = Source::makeOrGetASource(sourceWebpage);
    return source->getContentAndView(title);
    // return std::string();
}

Category* getHottestCategory(){
    return Category::getHighestCategory();
}

std::list<Category*> Category::categories;
std::map<Category*, int> Category::CategoryViewCount;

//
//int main() {
//    string title = "Something Happened";
//    string text = "London is a city where things happen all the time";
//    string author = "A random stranger";
//    string categoryName = "London News";
//    string sourceWebpage = "http://www.blameberg.com";
//
//    insertNewsItem(title, text, author, categoryName, sourceWebpage);
//    insertNewsItem("title", text, author, "categoryName", "sourceWebpage");
//    insertNewsItem("title", text, author, "categoryName", "sourceWebpage");
////
//    //sourceWebpage = "http://www.blameberg.com";
//    // title = "Something Happened";
//    string content = getContent(sourceWebpage, title);
//    getContent("sourceWebpage", "title");
//    getContent("sourceWebpage", "title");
//    cout << "Here is the content: " << content << endl;
//
//    Category* hottestCategory = getHottestCategory();
//    cout << hottestCategory->getName() << endl;
//
//    return 0;
//}


