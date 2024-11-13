/*
The Observer pattern is a behavioral design pattern that establishes a one-to-many dependency (one subject and many observers) between objects.
In this pattern, when one object (the subject) changes its state, all its dependents (observers) are notified and updated automatically.

Components involved:

Subject: It holds the state being observed. It maintains a list of observers and provides methods for subscribing, unsubscribing and notifying observers.
Observer: This is the interface that defines the update method, which the subject calls to notify observers of any state changes.
ConcreteSubject: This is a subclass of the subject class, which implements the specific behavior of subscribing, unsubscribing, and notifying observers.
    It also contains the actual state that observers are interested in.
ConcreteObserver: This is a subclass of the observer interface that implements the update method.
    Each concrete observer can define its own behavior for handling updates from the subject.

*/

// EXAMPLE 1

// Observer interface
class Observer {
public:
    virtual void update(const string& message) = 0;
};

// Concrete Observer
class ConcreteObserver : public Observer {
public:
    void update(const string& message) override {
        cout << "Received message: " << message << endl;
    }
};

// Publisher (Subject) class
class Publisher {
private:
    vector<Observer*> observers;

public:
    // Attach an observer
    void subscribe(Observer* observer) {
        observers.push_back(observer);
    }

    // Detach an observer
    void unsubscribe(Observer* observer) {
        // remove observer from the vector
    }

    // Notify all observers
    void notify(const string& message) {
        for (Observer* observer : observers) {
            observer->update(message);
        }
    }
};

int main() {
    Publisher publisher;

    // Create subscribers
    ConcreteSubscriber subscriber1;
    ConcreteSubscriber subscriber2;

    // Attach subscribers to the publisher
    publisher.subscribe(&subscriber1);
    publisher.subscribe(&subscriber2);

    // Publish messages
    publisher.publish("Message 1");
    publisher.publish("Message 2");

    return 0;
}

// EXAMPLE 2 - facebook group chat
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const string& newMessage) = 0;
};

// Subject interface
class Subject {
public:
    virtual ~Subject() = default;
    virtual void subscribe(Observer* observer) = 0;
    virtual void unsubscribe(Observer* observer) = 0;
    virtual void notify() = 0;
};

// Concrete Subject class
class GroupChat : public Subject {
private:
    vector<Observer*> observers;
    vector<string> messages;
public:
    void subscribe(Observer* observer) override {
        observers.push_back(observer);
    }

    void unsubscribe(Observer* observer) override {
        auto it = find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }
    }

    void notify() override {
        for (Observer* observer : observers) {
            observer->update(messages.back());
        }
    }

    void sendMessage(const string& msg) {
        messages.push_back(msg);
        notify();
    }
};

// Concrete Observer class
class Friend : public Observer {
private:
    string name;
public:
    Friend(const string& n) : name(n) {}

    void update(const string& newMessage) override {
        cout << name << " received a message: " << newMessage << endl;
    }
};

int main() {
    GroupChat chat;

    Friend friend1("Alice");
    Friend friend2("Bob");
    Friend friend3("Charlie");

    chat.subscribe(&friend1);
    chat.subscribe(&friend2);
    chat.subscribe(&friend3);

    chat.sendMessage("Hey everyone, let's plan our trip!");

    return 0;
}

