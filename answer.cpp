#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <cmath>
#include <set>
#include <cassert>
#include <utility>
#include <list>

using namespace std;

class DateTime {
private:
    int day, month, year, hour, minute;

    double days_since_01011970() const {
        // exploits C function mktime ("man mktime" for details)
        struct tm t;
        t.tm_mday = day;
        t.tm_mon = month - 1;
        t.tm_year = year - 1900;
        t.tm_sec = 0;
        t.tm_min = minute;
        t.tm_hour = hour;
        time_t t_seconds = mktime(&t);
        return t_seconds/86400.0;
    }

public:
    DateTime(int d, int m, int y, int hr=0, int mn=0) {
        day = d;
        month = m;
        year = y;
        hour = hr;
        minute = mn;
    }

    // copy constructor
    DateTime(const DateTime &other) : day(other.day), month(other.month), year(other.year), hour(other.hour), minute(other.minute) {
    }

    // make global operator for outputting dates to streams
    // a friend of the class
    friend ostream &operator<<(ostream &out, const DateTime &d);

    //  operator belonging to class for subtracting dates
    double operator-(const DateTime &other) const {
        return days_since_01011970() - other.days_since_01011970();
    }

    static DateTime now() {
        time_t rawtime;
        time(&rawtime);
        struct tm *t = localtime(&rawtime);
        assert(t);
        return DateTime(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
    }

};

ostream &operator<<(ostream &out, const DateTime &d) {
    out << d.day << "/" << d.month << "/" << d.year << " " << setw(2) << setfill('0') << d.hour << ":" << setw(2)  << d.minute << endl;;
    return out;
}

/* part a: write your declarations for Agent, TrainingCourse and Booking classes here */
class Booking;
class TrainingCourse;

class Agent {
private:
    string codeName;
    int securityLevel;

public:
    Agent(string _codeName, int _level);
    Booking* book(TrainingCourse* trainingCourse);
    bool attend(TrainingCourse& trainingCourse);
    int getSecurityLevel() const;
};

Agent::Agent(string _codeName, int _level): codeName(std::move(_codeName)), securityLevel(_level) {

}


int Agent::getSecurityLevel() const {
    return securityLevel;
}

class TrainingCourse {
private:
    string title;
    int capacity; // uint
    int minSecurityLevel;
    DateTime time;
    int currentCapacity = 0;
    // std::list<Agent*> attendingAgents;


public:
    TrainingCourse(string _title, DateTime& _time, int _capacity, int _minLevel);

    bool isFull() const;
    Booking* book(Agent& agent);
    bool cancel(Agent* agent);
    const DateTime& getStart();

    bool eligibleToAttend(Agent &agent, const DateTime &bookingTime);

};

Booking *Agent::book(TrainingCourse *trainingCourse) {
    return trainingCourse->book(*this);
}

bool Agent::attend(TrainingCourse &trainingCourse) {
    // This shouldnt be - should be booking time.
    return trainingCourse.eligibleToAttend(*this, DateTime::now());
}

class Booking {
private:
    Agent agent;
    TrainingCourse course;

public:
    explicit Booking(const DateTime& _time, Agent  _agent, TrainingCourse  _course);

    bool cancel(Agent* _agent);
    DateTime dateTime;

};

bool Booking::cancel(Agent *_agent) {
    // delete the memory? - tell course to remove the agent??
    return (course.cancel(_agent) && &agent == _agent);
}

Booking::Booking(const DateTime &_time, Agent _agent, TrainingCourse  _course) : dateTime(_time), agent(std::move(_agent)), course(std::move(_course)){}

// capacity should be uint - but compiler error?
TrainingCourse::TrainingCourse(string _title, DateTime &_time, int _capacity, int _minLevel) : title(std::move(_title)), time(_time), capacity(_capacity), minSecurityLevel(_minLevel){

}

bool TrainingCourse::isFull() const {
    return currentCapacity < capacity;
}

Booking *TrainingCourse::book(Agent &agent) {
    DateTime bookingTime = DateTime::now();
    if (eligibleToAttend(agent, bookingTime)){
        cout << "Agent cannot book the course" << endl;
        return nullptr;
    }
    currentCapacity++;
    // attendingAgents.push_back(&agent);
    cout << "Agent can book for the course" << endl;
    return new Booking(bookingTime, agent, *this);
}

bool TrainingCourse::eligibleToAttend(Agent &agent, const DateTime &bookingTime) { return agent.getSecurityLevel() <
                                                                                          minSecurityLevel || (getStart() - bookingTime) > 0 || isFull(); }

bool TrainingCourse::cancel(Agent *agent) {
    // okay to decrement as started? - or would need an extra method to set it from booking
    currentCapacity--;
    return ((DateTime::now() - getStart()) < 0);
}

const DateTime &TrainingCourse::getStart() {
    return time;
}

//int main(){
//    DateTime courseTime = DateTime(10, 2, 2023, 19, 0);
//    TrainingCourse advancedHackingCourse = TrainingCourse("Advanced Hacking", courseTime, 30, 1);
//    Agent bond = Agent("007", 2);
//    Agent will = Agent("Bald Eagle", 0);
//    Booking* bondBooking = advancedHackingCourse.book(bond);
//    Booking* willBooking = advancedHackingCourse.book(will);
//    bondBooking->cancel(&bond);
//    bool canBondAttend = bond.attend(advancedHackingCourse);
//
//    cout << "test";
//    return 0;
//}






