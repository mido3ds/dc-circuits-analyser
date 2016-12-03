#include "Data.h"

/*      Circuit     */

// private:
bool Circuit::_IsIt(Node* ptr, const double &val, SEARCH_BY type)
{
    switch (type)
    {
        case ID:
            return (val == ptr->GetId());
        case VOLT:
            return (val == ptr->GetVolt());
        default:
            throw -1;
    }
}

// i don't know why i wrote it, but maybe Hadi asks for it , who knows :D
void Circuit::_RemoveDuplicates()
{
    Node* first = _firstNode;

    // iterate through list, let every one of them be 'first' in its own sublist
    while (first)    
    {
        // to point at others 
        Node* other = first->_next;

        // iterate through all list to check duplicates of first
        while (other)
        {
            // there is a duplicate, remove it
            if (other->GetId() == first->GetId())
            {
                Node* temp = other;
                other = other->_next;

                Remove(temp);

                continue;
            }

            other = other->_next;
        }

        first = first->GetNext();
    }   
}

void Circuit::_Copy_this_toMe(Circuit* c)
{
     // copy nodes
    Node* originalNode = c->GetFirstNode();

    // traverse through them all till null
    while (originalNode)
    {
        Node* copiedNode = originalNode->Copy();
        Add(copiedNode);

        // go to the next
        originalNode = originalNode->GetNext();
    }
}

// remove duplicate elements/nodes,
// sources that connect to more than two node 
// or have the same sign/direction with two nodes
// make voltage sources at the first of list
bool Circuit::_Repair()
{
    bool needRepairs = false;

    // traverse through all nodes
    Node* first = _firstNode;

    // list to store elements 
    List l;

    while (first)
    {
        // repair this node first
        first->_Repair(l);

        // check duplicates of first
        Node* other = first->_next;
        while (other)
        {
            // there is a duplicate, remove it
            if (other->GetId() == first->GetId())
            {
                cerr << "===> ERROR! Found Node Dupliacte,Removeing it..\n";

                Node* temp = other;
                other = other->_next;
                Remove(temp);

                needRepairs = true;

                continue;
            }

            other = other->_next;
        }

        // move first to next
        first = first->_next;
    }

    return needRepairs;
}

//  public:

// Deconstructor
Circuit::~Circuit()
{
    while (Remove(_firstNode));
}

// Constructor
Circuit::Circuit()
	:_firstNode(nullptr), _lastNode(nullptr), _numNodes(0)
	{}

Circuit::Circuit(Circuit* c)
{
    _Copy_this_toMe(c);
}

Circuit::Circuit(Circuit& c)
{
    _Copy_this_toMe(&c);
}

// read the whole circuit from the user
void Circuit::Read()
{
    cout << "Please enter the elements node by node \nTo end the node type x \nTo end all nodes type xx\n";

    // read nodes
    bool read = true;
    for (int nodeI = 1; read; nodeI++)
    {
        cout << "Node #" << nodeI << ":\n";

        Node* newNode = new Node(nodeI);

        // variables to store the element
        char type;
        int id;
        double val;
        
        // read elements
        while (true)
        {
            // get first character 
            type = cin.get();

            // user entered x
            if (toupper(type) == 'X')   
            {
                // if user typed another x, end all ciruit
                type = cin.get();
                if (toupper(type) == 'X') 
                    read = false;
                
                break;
            }

            // user entered element
            else 
            {
                cin >> id >> val;

                Element* e = new Element(type, id, val);

                newNode->Add(e);
            }
        }

        // if node is empty, delete it and tell user that it's not added
        if (newNode->IsEmpty())
        {
            cout << "====> Node is empty, node will be deleted\n";
            delete newNode;
        }
        else if (newNode->GetNumOfElements() == 1)  // has one node
        {
            cout << "====> Warning: Node has one element, node will be deleted\n";
            delete newNode;
        }
        else        // not empty, add it 
            Add(newNode);
    }

    // look for invalid elements
    //


    if (IsEmpty())
    {
        cerr << "====> ERROR! Circuit is Empty \nCan not proceed, Please rerun the program\n";
        throw -1;
    }
}

void Circuit::Add(Node* n)
{
    /* pseudo:
        if essential:
            put at front of list
        else
            put at end of list

        put:
            ;let the pointer be ptr
            ptr.next points at n
            or -----> n.next points at ptr & ptr 
    */

    // handle Special case, when last and first are nullptrs
    if (n->IsEssential())
        Push_front(n);
    else 
        Push_back(n);
}

bool Circuit::Remove(Node* n)
{
    /*  pseudo:
        get previous node:
            start from beginning
            iterate until this.next = n
            if finsished without finding it, throw exception
        ;let prev = n-, this = n
        n-.next = n.next
        delete n

        ;check if n- exists

        ;special cases:
        ;n = firstnode ----> +
        ;n = lastnode -----> +
        ;n is not in list ---->  +
    */
    if (!n)
        return false;       // cant delete null

    // node is on edge: first/last
    if (n == _firstNode)
        return Pop_front();
    else if (n == _lastNode)
        return Pop_back();

    // node is in middle
    Node* &n_minus = n->_prev;
    Node* &n_plus = n->_next;

    n_minus->_next = n_plus;
    n_plus->_prev = n_minus;
    delete n;
    _numNodes--;

    return true;
}

Node* Circuit::GetLastNode()
{
    return (_lastNode);
}

Node* Circuit::GetFirstNode()
{
    return (_firstNode);
}

int Circuit::GetNumOfNodes()
{
    return(_numNodes);
}

void Circuit::Push_back(Node* n)
{
    if (!n)
        throw -1;       // cant handle empty pointer

    n->_next = nullptr;

    // handle special case, when first time pushing
    if (!_lastNode && !_firstNode)
    {
        _lastNode = _firstNode = n;
        n->_prev = nullptr;
        _numNodes++;
        return;
    }

    _lastNode->_next = n;
    n->_prev = _lastNode;
    _lastNode = n;
    _numNodes++;
}

void Circuit::Push_front(Node* n)
{
    if (!n)
        throw -1;       // cant handle empty pointer

    n->_prev = nullptr;

    if (!_lastNode && !_firstNode)
    {
        _lastNode = _firstNode = n;
        n->_next = nullptr;
        _numNodes++;
        return;
    }   

    n->_next = _firstNode;
    _firstNode->_prev = n; 
    _firstNode = n;
    _numNodes++;
}

bool Circuit::Pop_back()
{
    if (!_lastNode)
        return false;

    Node* temp = _lastNode;

    // if there is only one node, move both to null after removing it
    if (_lastNode == _firstNode)
        _lastNode = nullptr; 
        
    _lastNode = _lastNode->_prev;
    _lastNode->_next = nullptr;
    delete temp;
    _numNodes--;

    return true;
}

bool Circuit::Pop_front()
{
    if (!_firstNode)
        return false;

    Node* temp = _firstNode;

    // if there is only one node, move both to null after removing it
    if (_lastNode == _firstNode)
        _lastNode = nullptr;     

    _firstNode = _firstNode->_next;
    _firstNode->_prev = nullptr;
    delete temp;
    _numNodes--;

    return true;
}

bool Circuit::Remove(const double &val, SEARCH_BY type)
{
    Node* temp = _firstNode;

    // iterate through list 
    while (!_IsIt(temp, val, type))
    {
        temp = temp->GetNext();
        if (!temp)  // not found
            return false;
    }

    return Remove(temp);
}

Node* Circuit::GetNode(const double &val, SEARCH_BY type)
{
     Node* temp = _firstNode;

    // iterate through list 
    while (!_IsIt(temp, val, type))
    {
        temp = temp->GetNext();
        if (!temp)  // not found
            return nullptr;
    }

    return temp;
}

bool Circuit::HasNode(const double &val, SEARCH_BY type)
{
    Node* temp = _firstNode;

    // iterate through list 
    while (!_IsIt(temp, val, type))
    {
        temp = temp->GetNext();
        if (!temp)  // not found
            return false;
    }

    return true;
}

bool Circuit::IsEmpty()
{
    return (_firstNode == nullptr && _lastNode == nullptr);
}

// returns address of new circuit that is deepCopied of this
Circuit* Circuit::Copy()
{
    // allocate data
    Circuit* newCirc = new Circuit;

    // copy nodes
    Node* originalNode = _firstNode;

    // traverse through them all till null
    while (originalNode)
    {
        Node* copiedNode = originalNode->Copy();
        newCirc->Add(copiedNode);

        // go to the next
        originalNode = originalNode->_next;
    }

    // here is your copy
    return newCirc;
}

// operator overloading for copy
Circuit& Circuit::operator= (Circuit &c)
{
    return (*c.Copy());
}