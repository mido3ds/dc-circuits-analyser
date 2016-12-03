#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
using namespace std;

#ifndef NULL
#define NULL nullptr
#endif

// used in circuit methods
enum SEARCH_BY {ID, VOLT};

// element's type
enum Type {R, E, J};

/* PROTOTYPES */
class Element
{
private:
    Element* _next;
    Element* _prev;
    Type _type;
    int _id;
    double _value;

    void _SetType(const char &c);
    void _SetValue(const double &num);
    void _SetId(const int &id);

    class _List
    {
        //         type, id, val
        vector<tuple<char, int, double>> l;
    public:
        bool Add(Element* e)
        {
            auto tu = make_tuple(e->GetType, e->GetId, e->GetValue);

            // search for one like this 
            int occ = 0;
            for (int i = l.size(); i >= 0; i--)
            {
                // if same type and id 
                if (get<0>(l[i]) == get<0>(tu) && get<1>(l[i]) == get<1>(tu))
                {
                    occ++;

                    // if not a resistence
                    if (e->GetType() != 'R')
                    {
                        // if the same polarity, give error
                        if (get<2>(l[i]) == get<2>(tu))
                        {
                            cerr << "====> ERROR! Found Current/Voltage Source with the same polarity on two nodes, Removing it...\n";

                        }
                    }
                }
            }
        } 
    }

public:
    Element(const char &type, const int &id, const double &val);
    char GetType();
    void ChangeType(const char &c);
    Element* GetNext();
    Element* GetPrev();
    int GetId();
    void ChangeId(const int &num);
    double GetValue();
    void ChangeValue(const int &num);

    Element* Copy();
    bool operator== (Element&);

    // friends
    friend class Node;
    friend class Circuit;
};

class Node 
{
private:
    Node* _next;
    Node* _prev;
    Element* _firstElement;
    const int _id;
    double _volt;
    int _numElements;

    bool _Repair(List &l);

public:
    int GetId();
    void ChangeVolt(const double &v);
    double GetVolt();
    int GetNumOfElements();
    bool IsEssential();
    bool IsEmpty();
    Element* GetFirstElement();
    Node* GetNext();
    void Add(Element* e);
    bool Remove(Element* e);
    Node(const int &id);
	~Node();
    Node* Copy();

    // friends 
    friend class Circuit; 
};

class Circuit 
{
private:
    Node* _firstNode;
    Node* _lastNode;
	int _numNodes;

    bool _IsIt(Node* ptr, const double &val, SEARCH_BY type);
    void _RemoveDuplicates();
    void _Copy_this_toMe(Circuit*);

    bool _Repair();

public:
    void Add(Node* n);
    bool Remove(Node* n);
    Node* GetLastNode();
    Node* GetFirstNode();
	int GetNumOfNodes();
    void Read();
	~Circuit();
	Circuit();
    Circuit(Circuit* c);
    Circuit(Circuit& c);
	void Push_back(Node* n);
	void Push_front(Node* n);
	bool Pop_back();
	bool Pop_front();
    bool Remove(const double &val, SEARCH_BY type = ID);
    Node* GetNode(const double &val, SEARCH_BY type = ID);
    bool HasNode(const double &val, SEARCH_BY type = ID);
    bool IsEmpty();
    Circuit& operator= (Circuit &c);
    Circuit* Copy();
};