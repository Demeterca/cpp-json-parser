#include "json.hpp"


template<typename Val>
class list {
    /* private struct node holding the values in the list */
    struct node {
        Val val;
        node *next;
    };

public:

    /* default constructor */
    list();

    /* constructor with one value v */
    list(Val v);

    /* copy constructor */
    list(list<Val> const &rhs);

    /* move constructor */
    list(list<Val> &&rhs);

    /* destructor */
    ~list();

    /* removes all the elements */
    void clear();

    /* return a const* to first node */
    node const *cfront() const;

    /* return a const* to last node */
    node const *cback() const;

    node *front() const;

    node *back() const;

    /* push to the front of the list a new element with value v */
    void push_front(Val v);

    /* push to the back of the list a new element with value v */
    void push_back(Val v);

    /* remove first element */
    void pop_front();

    /* remove last element */
    void pop_back();

    /* is the container empty? */
    bool empty() const;

    /* copy assignment operator:
    l1 = l2 overwrites the content of
    l1 with the content of l2, does not modify l2.
    At the end, returns a reference to l1. */
    list<Val> &operator=(list<Val> const &rhs);

    /* move assignment operator:
    l1 = l2 moves the content of
    l2 into l1.
    At the end, returns a reference to l1. */
    list<Val> &operator=(list<Val> &&rhs);

    /* is this container equal to rhs? */
    bool operator==(list<Val> const &rhs) const;

    /* is this container not equal to rhs? */
    bool operator!=(list<Val> const &rhs) const;

    /* Append to the back of the container the rhs
    and return a reference to *this.
    Why do we return a reference?
    Because we may want to write: l1 += l2 += l3; */
    list<Val> &operator+=(list<Val> const &rhs);

    /* appends v at the back of the container
    and return a reference to *this */
    list<Val> &operator+=(Val v);

    /* returns a const reference to the val of the i-th node if it exists */
    Val const &operator[](uint64_t i) const;

    /* returns a reference to the val of the i-th node if it exists */
    Val &operator[](uint64_t i);

    /* prefix decrement: remove head and return a reference to *this */
    list<Val> &operator--();

    /* postfix decrement: remove tail and return a reference to *this
       Note: the int parameter is a dummy parameter used to differentiate
       between prefix and postfix decrement. */
    list<Val> &operator--(int dummy);

    /* forward iterator */
    struct iterator {
        /* properties */
        using iterator_category = std::forward_iterator_tag;
        using value_type = Val;
        using pointer = Val *;
        using reference = Val &;

        /* constructor */
        iterator(node *ptr);

        /* we let the compiler define the destructor
           and copy constructor/assignment */

        reference operator*() const;

        /* Useful if Val is a struct with attributes.
           For example, if Val has an attribute x, we
           can write it->x.  */
        pointer operator->() const;

        /* prefix increment: advance the iterator to the next element
           and return it. */
        iterator &operator++();

        /* postfix increment: copy the iterator, advance the
           iterator to the next element, and return the copy. */
        iterator operator++(int /* dummy */);

        bool operator==(iterator const &rhs) const;

        bool operator!=(iterator const &rhs) const;

        operator bool() const;

    private:
        node *m_ptr;
    };

    using iterator = iterator;

    /* forward const_iterator */
    struct const_iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = const Val;
        using pointer = Val const *;
        using reference = Val const &;

        const_iterator(node *ptr);

        reference operator*() const;

        pointer operator->() const;

        const_iterator &operator++();

        const_iterator operator++(int /* dummy */);

        bool operator==(const_iterator const &rhs) const;

        bool operator!=(const_iterator const &rhs) const;

        operator bool() const;

    private:
        node const *m_ptr;
    };

    using const_iterator = const_iterator;
    /* return a new iterator pointing to the first element in the
       container -- the "beginning" of the container */
    // iterator begin();
    iterator begin();

    /* return a new iterator pointing to (one-past) the last element in the
       container -- the "end" of the container */
    iterator end();


    const_iterator cbegin() const;

    const_iterator cend() const;

private:
    node *m_front;  // pointer to first node: the "front" of the container
    node *m_back;   // pointer to last node: the "back" of the container
};


/* default constructor */
template<typename Val>
list<Val>::list() : m_front(nullptr), m_back(nullptr) {}

/* constructor with one value v */
template<typename Val>
list<Val>::list(Val v)
        : list()  // calls the default constructor
{
    push_front(v);
}

/* copy constructor */
template<typename Val>
list<Val>::list(list<Val> const &rhs)
        : list()  // calls the default constructor
{
    auto ptr = rhs.front();
    while (ptr) {
        push_back(ptr->val);
        ptr = ptr->next;
    }
    /* can also be re-written in terms of copy assignment operator,
    very elegantly, like this: */
    // *this = rhs;
}

/* copy assignment operator */
template<typename Val>
list<Val> &list<Val>::operator=(list<Val> const &rhs) {
    if (this != &rhs) {  // not a self-assignment
        /* first delete all items (and free the memory) */
        clear();
        auto ptr = rhs.front();
        while (ptr) {
            push_back(ptr->val);
            ptr = ptr->next;
        }
    }
    return *this;
}

/* move constructor */
template<typename Val>
list<Val>::list(list<Val> &&rhs)
        : list()  // calls the default constructor
{
    m_front = rhs.m_front;
    m_back = rhs.m_back;
    rhs.m_front = nullptr;
    rhs.m_back = nullptr;
    /* can also be re-written in terms of move assignment operator,
    very elegantly, like this: */
    // *this = std::move(rhs);
}

/* move assignment operator */
template<typename Val>
list<Val> &list<Val>::operator=(list<Val> &&rhs) {
    if (this != &rhs) {  // not a self-assignment
        /* first delete all items (and free the memory) */
        clear();
        m_front = rhs.m_front;
        m_back = rhs.m_back;
        rhs.m_front = nullptr;
        rhs.m_back = nullptr;
    }
    return *this;
}

/* is the container empty? */
template<typename Val>
bool list<Val>::empty() const {
    return m_front == nullptr;
}

/* remove first element */
template<typename Val>
void list<Val>::pop_front() {
    if (!empty()) {
        node *tmp = m_front;
        m_front = m_front->next;
        delete tmp;
        if (empty()) m_back = nullptr;
    }
}

/* remove last element */
template<typename Val>
void list<Val>::pop_back() {
    if (m_front == m_back) {
        pop_front();
        return;
    }
    /* we have at least 2 nodes in the list */
    /* take the pointer to the node just before m_back */
    node *prev = m_front;
    while (prev->next != m_back) prev = prev->next;
    /* delete m_back */
    delete m_back;
    /* adjust m_back */
    prev->next = nullptr;
    m_back = prev;
}

/* clear() */
template<typename Val>
void list<Val>::clear() {
    while (!empty()) pop_front();
}

/* destructor */
template<typename Val>
list<Val>::~list() {
    clear();
}

/* return a const* to first node */
template<typename Val>
typename list<Val>::node const *list<Val>::cfront() const {
    return m_front;
}

/* return a const* to last node */
template<typename Val>
typename list<Val>::node const *list<Val>::cback() const {
    return m_back;
}

template<typename Val>
typename list<Val>::node *list<Val>::front() const {
    return m_front;
}

template<typename Val>
typename list<Val>::node *list<Val>::back() const {
    return m_back;
}

/* push to the front of the list a new element with value v */
template<typename Val>
void list<Val>::push_front(Val v) {
    if (empty()) {
        m_front = new node{v, nullptr};
        m_back = m_front;
        return;
    }
    m_front = new node{v, m_front};
}

/* push to the back of the list a new element with value v */
template<typename Val>
void list<Val>::push_back(Val v) {
    if (empty()) {
        push_front(v);
        return;
    }
    m_back->next = new node{v, nullptr};
    m_back = m_back->next;
}

/* prefix decrement: remove the first node and return a reference to *this */
template<typename Val>
list<Val> &list<Val>::operator--() {
    pop_front();
    return *this;
}

/* postfix decrement: remove the last node and return a reference to *this
   Note:
   the int parameter is a dummy parameter used to differentiate between
   prefix and postfix decrement. */
template<typename Val>
list<Val> &list<Val>::operator--(int /* dummy */) {
    pop_back();
    return *this;
}

/* is this container equal to rhs? */
template<typename Val>
bool list<Val>::operator==(list<Val> const &rhs) const {

    auto it1 = begin();
    auto it2 = rhs.begin();
    while (it1 or it2) {
        if ((!it1 and it2) or (it1 and !it2) or (*it1 != *it2)) {
            return false;
        }
        it1++;
        it2++;
    }
    return true;
}

/* is this container not equal to rhs? */
template<typename Val>
bool list<Val>::operator!=(list<Val> const &rhs) const {
    return !((*this) == rhs);
}

/* append to the back of the container the rhs
and return a reference to *this */
template<typename Val>
list<Val> &list<Val>::operator+=(list<Val> const &rhs) {
    node const *ptr = rhs.front();
    while (ptr) {
        push_back(ptr->val);
        ptr = ptr->next;
    }
    return *this;
}

/* push v to the back of the container
and return a reference to *this */
template<typename Val>
list<Val> &list<Val>::operator+=(Val v) {
    push_back(v);
    return *this;
}

/* returns a const reference to the val of the i-th node if it exists */
template<typename Val>
Val const &list<Val>::operator[](uint64_t i) const {
    uint64_t pos = 0;
    node const *ptr = m_front;
    while (pos++ < i) ptr = ptr->next;
    return ptr->val;
}

/* returns a reference to the val of the i-th node if it exists */
template<typename Val>
Val &list<Val>::operator[](uint64_t i) {
    uint64_t pos = 0;
    node const *ptr = m_front;
    while (pos++ < i) ptr = ptr->next;
    return ptr->val;
}

/*
    iterator implementation
*/

template<typename Val>
list<Val>::iterator::iterator(node *ptr) : m_ptr(ptr) {}

template<typename Val>
typename list<Val>::iterator::reference list<Val>::iterator::operator*() const {
    return m_ptr->val;
}

template<typename Val>
typename list<Val>::iterator::pointer list<Val>::iterator::operator->() const {
    return &(m_ptr->val);
}

template<typename Val>
typename list<Val>::iterator &list<Val>::iterator::operator++() {
    m_ptr = m_ptr->next;
    return *this;
}

template<typename Val>
typename list<Val>::iterator list<Val>::iterator::operator++(int /* dummy */) {
    iterator copy = *this;
    m_ptr = m_ptr->next;
    return copy;
}

template<typename Val>
bool list<Val>::iterator::operator==(
        typename list<Val>::iterator const &rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename Val>
bool list<Val>::iterator::operator!=(
        typename list<Val>::iterator const &rhs) const {
    return !(*this == rhs);
}

template<typename Val>
list<Val>::iterator::operator bool() const {
    return m_ptr != nullptr;  // or just: return m_ptr
}

template<typename Val>
typename list<Val>::iterator list<Val>::begin() {
    return {m_front};
}

template<typename Val>
typename list<Val>::iterator list<Val>::end() {
    /* note: we do not return {m_back} because
       this will be an iterator pointing to the last element.
       We return an iterator pointing to one-past the last element,
       thus, iterator(nullptr). */
    return {nullptr};
}

/*
    const_iterator implementation
*/

template<typename Val>
list<Val>::const_iterator::const_iterator(node *ptr) : m_ptr(ptr) {}

template<typename Val>
typename list<Val>::const_iterator::reference
list<Val>::const_iterator::operator*() const {
    return m_ptr->val;
}

template<typename Val>
typename list<Val>::const_iterator::pointer
list<Val>::const_iterator::operator->() const {
    return &(m_ptr->val);
}

template<typename Val>
typename list<Val>::const_iterator &list<Val>::const_iterator::operator++() {
    m_ptr = m_ptr->next;
    return *this;
}

template<typename Val>
typename list<Val>::const_iterator list<Val>::const_iterator::operator++(
        int /* dummy */) {
    const_iterator copy = *this;
    m_ptr = m_ptr->next;
    return copy;
}

template<typename Val>
bool list<Val>::const_iterator::operator==(
        typename list<Val>::const_iterator const &rhs) const {
    return m_ptr == rhs.m_ptr;
}

template<typename Val>
bool list<Val>::const_iterator::operator!=(
        typename list<Val>::const_iterator const &rhs) const {
    return !(*this == rhs);
}

template<typename Val>
list<Val>::const_iterator::operator bool() const {
    return m_ptr != nullptr;  // or just: return m_ptr
}

template<typename Val>
typename list<Val>::const_iterator list<Val>::cbegin() const {
    return {m_front};
}

template<typename Val>
typename list<Val>::const_iterator list<Val>::cend() const {
    /* note: we do not return {m_back} because
       this will be an iterator pointing to the last element.
       We return an iterator pointing to one-past the last element,
       thus, iterator(nullptr). */
    return {nullptr};
}

/*
    Non member functions.
*/

/* return lhs+rhs where both lhs and rhs are lists */
template<typename Val>
list<Val> operator+(list<Val> const &lhs, list<Val> const &rhs) {
    list<Val> result = lhs;  // copy constructor
    result += rhs;
    return result;
}

/* return lhs+rhs where rhs is a value */
template<typename Val>
list<Val> operator+(list<Val> const &lhs, Val rhs) {
    list<Val> res{lhs};
    res.push_back(rhs);
    return res;
}

/* return lhs+rhs where lhs is a value */
template<typename Val>
list<Val> operator+(Val lhs, list<Val> const &rhs) {
    list<Val> res{rhs};
    res.push_front(lhs);
    return res;
}

template<typename Val>
std::ostream &operator<<(std::ostream &os, list<Val> const &list) {
    for (auto ptr = list.front(); ptr; ptr = ptr->next) {
        os << ptr->val;
        if (ptr->next) os << " -> ";
    }
    return os;
}

template<typename Val>
std::istream &operator>>(std::istream &is, list <Val> &list) {
    Val v;
    while (is >> v) list.push_back(v);
    return is;
}

template<typename Val>
void read_list(list<Val> &list) {
    uint64_t list_size = 0;
    Val v;
    std::cin >> list_size;                       // first read list_size
    for (uint64_t i = 0; i != list_size; ++i) {  // then value by value
        std::cin >> v;
        list.push_back(v);
    }
}

struct json::impl {
    double *number = nullptr;
    bool *b = nullptr;
    std::string *s = nullptr;
    list<json> *l = nullptr;
    list<std::pair<std::string, json>> *dict = nullptr;

    void clear() {
        if (number != nullptr)
            delete number;
        else if (b != nullptr)
            delete b;
        else if (s != nullptr)
            delete s;
        else if (l != nullptr)
            delete l;
        else if (dict != nullptr)
            delete dict;
    }

};

bool json::is_list() const {
    return this->pimpl->l != nullptr;
}

bool json::is_dictionary() const {
    return this->pimpl->dict != nullptr;
}

bool json::is_string() const {
    return this->pimpl->s != nullptr;
}

bool json::is_number() const {
    return this->pimpl->number != nullptr;
}

bool json::is_bool() const {
    return this->pimpl->b != nullptr;
}

bool json::is_null() const {
    return !this->is_list() && !this->is_dictionary() && !this->is_string() && !this->is_number() && !this->is_bool();
}

struct json::list_iterator {
    using iterator_category = std::forward_iterator_tag;    //stub dell'iteratore delle liste
    using value_type = list<json>::iterator::value_type;
    using pointer = list<json>::iterator::pointer;
    using reference = list<json>::iterator::reference;

    /* constructor */
    list_iterator(list<json>::iterator _it) : it(_it) {}

    /* we let the compiler define the destructor
       and copy constructor/assignment */

    reference operator*() const {
        return *this->it;
    }

    /* ritorna puntatore all'elemento corrente della lista  */
    pointer operator->() const {
        return &(*this->it);
    }

    /* prefix increment: advance the iterator to the next element
       and return it. */
    list_iterator &operator++() {
        ++(this->it);
        return *this;
    }

    /* postfix increment: copy the iterator, advance the
       iterator to the next element, and return the copy. */
    list_iterator operator++(int /* dummy */) {
        list_iterator copy = *this;
        this->it = ++(this->it);
        return copy;
    }

    bool operator==(list_iterator const &rhs) const {
        return this->it == rhs.it;
    }


    bool operator!=(list_iterator const &rhs) const {
        return !(this->it == rhs.it);
    }

private:
    list<json>::iterator it;
};


struct json::const_list_iterator {
    using iterator_category = std::forward_iterator_tag;    //stub dell'iteratore delle liste
    using value_type = list<json>::const_iterator::value_type;
    using const_pointer = list<json>::const_iterator::pointer;
    using const_reference = list<json>::const_iterator::reference;

    /* constructor */
    const_list_iterator(const list<json>::const_iterator _it) : it(_it) {}

    /* we let the compiler define the destructor
       and copy constructor/assignment */

    const_reference operator*() const {
        return *this->it;
    }

    /* ritorna puntatore all'elemento corrente della lista  */
    const_pointer operator->() const {
        return &(*this->it);
    }

    /* prefix increment: advance the iterator to the next element
       and return it. */
    const_list_iterator &operator++() {
        ++(this->it);
        return *this;
    }

    /* postfix increment: copy the iterator, advance the
       iterator to the next element, and return the copy. */
    const_list_iterator operator++(int /* dummy */) {
        const_list_iterator copy = *this;
        this->it = ++(this->it);
        return copy;
    }

    bool operator==(const_list_iterator const &rhs) const {
        return this->it == rhs.it;
    }


    bool operator!=(const_list_iterator const &rhs) const {
        return !(*this == rhs); //!(this->it == rhs.it);
    }

private:
    list<json>::const_iterator it;
};


struct json::dictionary_iterator {
    using iterator_category = std::forward_iterator_tag;    //stub dell'iteratore delle liste
    using value_type = list<std::pair<std::string, json>>::iterator::value_type;
    using pointer = list<std::pair<std::string, json>>::iterator::pointer;
    using reference = list<std::pair<std::string, json>>::iterator::reference;

    /* constructor */
    dictionary_iterator(const list<std::pair<std::string, json>>::iterator _it) : it(_it) {}

    /* we let the compiler define the destructor
       and copy constructor/assignment */

    reference operator*() const {
        return *this->it;
    }

    /* ritorna puntatore all'elemento corrente della lista  */
    pointer operator->() const {
        return &(*this->it);
    }

    /* prefix increment: advance the iterator to the next element
       and return it. */
    dictionary_iterator &operator++() {
        ++(this->it);
        return *this;
    }

    /* postfix increment: copy the iterator, advance the
       iterator to the next element, and return the copy. */
    dictionary_iterator operator++(int /* dummy */) {
        dictionary_iterator copy = *this;
        this->it = ++(this->it);
        return copy;
    }

    bool operator==(dictionary_iterator const &rhs) const {
        return this->it == rhs.it;
    }


    bool operator!=(dictionary_iterator const &rhs) const {
        return !(this->it == rhs.it);
    }

private:
    list<std::pair<std::string, json>>::iterator it;
};


struct json::const_dictionary_iterator {
    using iterator_category = std::forward_iterator_tag;    //stub dell'iteratore delle liste
    using value_type = list<std::pair<std::string, json>>::const_iterator::value_type;
    using const_pointer = list<std::pair<std::string, json>>::const_iterator::pointer;
    using const_reference = list<std::pair<std::string, json>>::const_iterator::reference;

    /* constructor */
    const_dictionary_iterator(list<std::pair<std::string, json>>::const_iterator _it) : it(_it) {}

    /* we let the compiler define the destructor
       and copy constructor/assignment */

    const_reference operator*() const {
        return *this->it;
    }

    /* ritorna puntatore all'elemento corrente della lista  */
    const_pointer operator->() const {
        return &(*this->it);
    }

    /* prefix increment: advance the iterator to the next element
       and return it. */
    const_dictionary_iterator &operator++() {
        ++(this->it);
        return *this;
    }

    /* postfix increment: copy the iterator, advance the
       iterator to the next element, and return the copy. */
    const_dictionary_iterator operator++(int /* dummy */) {
        const_dictionary_iterator copy = *this;
        this->it = ++(this->it);
        return copy;
    }

    bool operator==(const_dictionary_iterator const &rhs) const {
        return this->it == rhs.it;
    }


    bool operator!=(const_dictionary_iterator const &rhs) const {
        return !(*this == rhs); //!(this->it == rhs.it);
    }

private:
    list<std::pair<std::string, json>>::const_iterator it;
};


json::list_iterator json::begin_list() {
    if (!this->is_list()) {
        std::cout << "riga 877" << std::endl;
        throw json_exception{"this is not a list"};
    }
    else {
        return list_iterator(this->pimpl->l->begin());
    }
}

json::list_iterator json::end_list() {
    if (!this->is_list()) {
        std::cout << "riga 887" << std::endl;
        throw json_exception{"this is not a list"};
    }
    else {
        return list_iterator(this->pimpl->l->end());
    }
}

json::const_list_iterator json::begin_list() const {
    if (!this->is_list()) {
        std::cout << "riga 897" << std::endl;
        throw json_exception{"this is not a list"};
    }
    else {
        return const_list_iterator(this->pimpl->l->cbegin());
    }
}

json::const_list_iterator json::end_list() const {
    if (!this->is_list()) {
        std::cout << "riga 907" << std::endl;
        throw json_exception{"this is not a list"};
    }
    else {
        return const_list_iterator(this->pimpl->l->cend());
    }
}


json::dictionary_iterator json::begin_dictionary() {
    if (!this->is_list()) {
        std::cout << "riga 918" << std::endl;
        throw json_exception{"this is not a list"};
    }
    else {
        return dictionary_iterator(this->pimpl->dict->begin());
    }
}

json::dictionary_iterator json::end_dictionary() {
    if (!this->is_list()) {
        std::cout << "riga 928" << std::endl;
        throw json_exception{"this is not a list"};
    }
    else {
        return dictionary_iterator(this->pimpl->dict->end());
    }
}

json::const_dictionary_iterator json::begin_dictionary() const {
    if (!this->is_dictionary()) {
        std::cout << "riga 938" << std::endl;
        throw json_exception{"this is not a dictionary"};
    }
    else {
        return const_dictionary_iterator(this->pimpl->dict->cbegin());
    }
}

json::const_dictionary_iterator json::end_dictionary() const {
    if (!this->is_dictionary()) {
        std::cout << "riga 948" << std::endl;
        throw json_exception{"this is not a dictionary"};
    }
    else {
        return const_dictionary_iterator(this->pimpl->dict->cend());
    }
}



json const &json::operator[](std::string const &kiave) const {
    if (!this->is_dictionary()){
        std::cout << "riga 969" << std::endl;
        throw json_exception{"this is not a dictionary"};
    }


    for (json::dictionary_iterator it = this->pimpl->dict->begin(); it != this->pimpl->dict->end(); it++) {
        if ((*it).first == kiave)
            return (*it).second;
    }
    std::cout << "riga 978" << std::endl;
    throw json_exception{"non posso effettuare l'inserimento"};
}

json &json::operator[](std::string const &kiave) {
    if (!this->is_dictionary()) {
        std::cout << "riga 984" << std::endl;
        throw json_exception{"this is not a dictionary"};
    }


    for (json::dictionary_iterator it = this->pimpl->dict->begin(); it != this->pimpl->dict->end(); it++) {
        if ((*it).first == kiave)
            return (*it).second;
    }
    this->pimpl->dict->push_back(std::pair<std::string, json>(kiave, json()));
    return this->pimpl->dict->back()->val.second;

}

json::json() : pimpl(new json::impl) {}

json::~json() {
    if (this->is_bool())
        delete this->pimpl->b;
    if (this->is_number())
        delete this->pimpl->number;
    if (this->is_string())
        delete this->pimpl->s;
    if (this->is_dictionary())
        delete this->pimpl->dict;
    if (this->is_list())
        delete this->pimpl->l;

    delete this->pimpl;
}

json::json(json const &j) : json() {
    if (j.is_bool())
        this->pimpl->b = new bool(*j.pimpl->b);
    if (j.is_number())
        this->pimpl->number = new double(*j.pimpl->number);
    if (j.is_string())
        this->pimpl->s = new std::string(*j.pimpl->s);
    if (j.is_dictionary()) {
        this->pimpl->dict = new list<std::pair<std::string, json>>(*j.pimpl->dict);
    }
    if (j.is_list()) {
        this->pimpl->l = new list<json>(*j.pimpl->l);
    }
}

json::json(json &&j) : json() {
    if (j.is_bool()) {
        this->pimpl->b = j.pimpl->b;
        j.pimpl->b = nullptr;
    }
    if (j.is_number()) {
        this->pimpl->number = j.pimpl->number;
        j.pimpl->number = nullptr;
    }
    if (j.is_string()) {
        this->pimpl->s = j.pimpl->s;
        j.pimpl->s = nullptr;
    }
    if (j.is_dictionary()) {
        this->pimpl->dict = j.pimpl->dict;
        j.pimpl->dict = nullptr;
    }
    if (j.is_list()) {
        this->pimpl->l = j.pimpl->l;
        j.pimpl->l = nullptr;
    }
}

json &json::operator=(json const &j) {
    if (this != &j) {  // not a self-assignment
        /* first delete all items (and free the memory) */
        this->pimpl->clear();
        if (j.is_bool())
            this->pimpl->b = new bool(*j.pimpl->b);
        if (j.is_number())
            this->pimpl->number = new double(*j.pimpl->number);
        if (j.is_string())
            this->pimpl->s = new std::string(*j.pimpl->s);
        if (j.is_dictionary()) {
            this->pimpl->dict = new list<std::pair<std::string, json>>(*j.pimpl->dict);
        }
        if (j.is_list()) {
            this->pimpl->l = new list<json>(*j.pimpl->l);
        }
    }
    return *this;
}


json &json::operator=(json &&j) {
    if (this != &j) {  // not a self-assignment
        /* first delete all items (and free the memory) */
        this->pimpl->clear();
        if (j.is_bool()) {
            this->pimpl->b = j.pimpl->b;
            j.pimpl->b = nullptr;
        }
        if (j.is_number()) {
            this->pimpl->number = j.pimpl->number;
            j.pimpl->number = nullptr;
        }
        if (j.is_string()) {
            this->pimpl->s = j.pimpl->s;
            j.pimpl->s = nullptr;
        }
        if (j.is_dictionary()) {
            this->pimpl->dict = j.pimpl->dict;
            j.pimpl->dict = nullptr;
        }
        if (j.is_list()) {
            this->pimpl->l = j.pimpl->l;
            j.pimpl->l = nullptr;
        }
    }
    return *this;
}


double &json::get_number() {
    if (this->is_number())
        return *(this->pimpl->number);
    else {
        std::cout << "riga 1107" << std::endl;
        throw json_exception{"this is not a number"};
    }
}

double const &json::get_number() const {
    if (this->is_number())
        return *(this->pimpl->number);
    else {
        std::cout << "riga 1116" << std::endl;
        throw json_exception{"this is not a number"};
    }
}

bool &json::get_bool() {
    if (this->is_bool())
        return *(this->pimpl->b);
    else {
        std::cout << "riga 1125" << std::endl;
        throw json_exception{"this is not a bool"};
    }
}

bool const &json::get_bool() const {
    if (this->is_bool())
        return *(this->pimpl->b);
    else {
        std::cout << "riga 1134" << std::endl;
        throw json_exception{"this is not a bool"};
    }
}

std::string &json::get_string() {
    if (this->is_string())
        return *(this->pimpl->s);
    else {
        std::cout << "riga 1143" << std::endl;
        throw json_exception{"this is not a string"};
    }
}

std::string const &json::get_string() const {
    if (this->is_string())
        return *(this->pimpl->s);
    else {
        std::cout << "riga 1152" << std::endl;
        throw json_exception{"this is not a string"};
    }
}

void json::set_string(const std::string &x) {
    if (this->is_number())
        delete this->pimpl->number;
    else if (this->is_list())
        delete this->pimpl->l;
    else if (this->is_bool())
        delete this->pimpl->b;
    else if (this->is_dictionary())
        delete this->pimpl->dict;
    else if (this->is_string())
        delete this->pimpl->s;

    this->pimpl->s = new std::string{x};
}

void json::set_bool(bool x) {
    if (this->is_number())
        delete this->pimpl->number;
    else if (this->is_list())
        delete this->pimpl->l;
    else if (this->is_bool())
        delete this->pimpl->b;
    else if (this->is_dictionary())
        delete this->pimpl->dict;
    else if (this->is_string())
        delete this->pimpl->s;

    this->pimpl->b = new bool{x};
}

void json::set_number(double x) {
    if (this->is_number())
        delete this->pimpl->number;
    else if (this->is_list())
        delete this->pimpl->l;
    else if (this->is_bool())
        delete this->pimpl->b;
    else if (this->is_dictionary())
        delete this->pimpl->dict;
    else if (this->is_string())
        delete this->pimpl->s;

    this->pimpl->number = new double{x};
}

void json::set_null() {
    if (this->is_number())
        delete this->pimpl->number;
    else if (this->is_list())
        delete this->pimpl->l;
    else if (this->is_bool())
        delete this->pimpl->b;
    else if (this->is_dictionary())
        delete this->pimpl->dict;
    else if (this->is_string())
        delete this->pimpl->s;
}

void json::set_list() {
    if (this->is_number())
        delete this->pimpl->number;
    else if (this->is_list())
        delete this->pimpl->l;
    else if (this->is_bool())
        delete this->pimpl->b;
    else if (this->is_dictionary())
        delete this->pimpl->dict;
    else if (this->is_string())
        delete this->pimpl->s;

    this->pimpl->l = new list<json>();
}

void json::set_dictionary() {
    if (this->is_number())
        delete this->pimpl->number;
    else if (this->is_list())
        delete this->pimpl->l;
    else if (this->is_bool())
        delete this->pimpl->b;
    else if (this->is_dictionary())
        delete this->pimpl->dict;
    else if (this->is_string())
        delete this->pimpl->s;

    this->pimpl->dict = new list<std::pair<std::string, json>>();
}

void json::push_front(const json &x) {
    if (this->is_list()) {
        this->pimpl->l->push_front(x);
    } else {
        std::cout << "riga 1249" << std::endl;
        throw json_exception{"this is not a list"};
    }
}

void json::push_back(const json &x) {
    if (this->is_list())
        this->pimpl->l->push_back(x);
    else {
        std::cout << "riga 1258" << std::endl;
        throw json_exception{"this is not a list"};
    }
}

void json::insert(const std::pair<std::string, json> &x) {
    if (this->is_dictionary()) {
        this->pimpl->dict->push_front(x);
    } else {
        std::cout << "riga 1267" << std::endl;
        throw json_exception{"this is not a dictionary"};
    }
}

std::ostream &operator<<(std::ostream &lhs, json const &rhs) {
    if (rhs.is_number()) {
        lhs << rhs.get_number();
    } else if (rhs.is_list()) {
        lhs << "[ ";
        for (json::const_list_iterator it = rhs.begin_list(); it != rhs.end_list(); it++) {
            lhs << *it << ", ";
        }
        lhs << "] ";
    } else if (rhs.is_dictionary()) {
        lhs << "{ ";
        for (json::const_dictionary_iterator it = rhs.begin_dictionary(); it != rhs.end_dictionary(); it++) {
            lhs << (*it).first << ":" << (*it).second << ", ";
        }
        lhs << "} ";
    } else if (rhs.is_string()) {
        lhs << rhs.get_string();
    } else if (rhs.is_bool()) {
        lhs << (rhs.get_bool() ? "true" : "false");
    } else {
        lhs << "null";
    }

    return lhs;
}





enum class TOKEN
{
    GRAFFA_APERTA = '{',
    GRAFFA_CHIUSA = '}',
    DUE_PUNTI = ':',
    STRING = 's',
    NUMBER = 'n',
    QUADRA_APERTA = '[',
    QUADRA_CHIUSA = ']',
    VIRGOLA = ',',
    BOOLEAN = 'b',
    NULLO = '0',
    FINE_INPUT = 'f',
};

struct Token
{
    std::string value;
    TOKEN type;
    std::string toString();
};


class Tokenizer{
public:
    std::string s;
    std::istream& is;
    char curr_char = ' ';

    Tokenizer(std::istream& input) : is(input) {}

    //Tokenizer(std::string st) : s(st) {}


    void getWithoutWhiteSpace();



    Token get_token();


};


void Tokenizer::getWithoutWhiteSpace() {

    while ((curr_char == ' ' || curr_char == '\n') && is >> curr_char) {


    }

}


Token Tokenizer::get_token() {
    struct Token token;
    //char c = ' ';
    //is.get(c);
    //std::cout<< curr_char <<std::endl;
    if(!(is >> curr_char)){
        token.type = TOKEN::FINE_INPUT;
        return token;
    }

    if(curr_char == ' ' || curr_char == '\n')
        getWithoutWhiteSpace();


    if (curr_char == '"') {
        token.type = TOKEN::STRING;
        token.value = "";

        is.get(curr_char);

        while (curr_char != '"') {

            token.value += curr_char;

            is.get(curr_char);
        }
    } else if (curr_char == '{') {
        token.type = TOKEN::GRAFFA_APERTA;
    } else if (curr_char == '}') {
        token.type = TOKEN::GRAFFA_CHIUSA;
    } else if (curr_char == '-' || (curr_char >= '0' && curr_char <= '9')) {
        //Check if string is numeric
        token.type = TOKEN::NUMBER;
        token.value = "";
        token.value += curr_char;
        //std::streampos prevCharPos = is.tellg();
        while ((curr_char == '-') || (curr_char >= '0' && curr_char <= '9') || curr_char == '.') {
            //prevCharPos = is.tellg();
            //is.get(c);
            curr_char = is.peek();

            if (!is.peek()) {
                break;
            } else {
                if ((curr_char == '-') || (curr_char >= '0' && curr_char <= '9') || (curr_char == '.')) {
                    token.value += curr_char;
                    is.get(curr_char);
                } else {
                    //is.seekg(prevCharPos);
                    // std::cout << c << std::endl;
                }
            }
        }
    } else if (curr_char == 'f') {
        token.type = TOKEN::BOOLEAN;
        token.value = "False";
        char buffer[5];

        is.get(buffer, 5);

        std::string data(buffer);
        if (data != "alse") {
            throw json_exception{"boolean not valid"};
        }
    } else if (curr_char == 't') {
        token.type = TOKEN::BOOLEAN;
        token.value = "True";
        char buffer[5];

        is.get(buffer, 4);

        std::string data(buffer);
        if (data != "rue") {
            throw json_exception{"boolean not valid"};
        }


    } else if (curr_char == 'n') {
        token.type = TOKEN::NULLO;
        is.seekg(3,std::ios_base::cur);
    } else if (curr_char == '[') {
        token.type = TOKEN::QUADRA_APERTA;
    } else if (curr_char == ']') {
        token.type = TOKEN::QUADRA_CHIUSA;
    } else if (curr_char == ':') {
        token.type = TOKEN::DUE_PUNTI;
    } else if (curr_char == ',') {
        token.type = TOKEN::VIRGOLA;
    }
    else{
        std::cout << "riga 1381 " << curr_char << std::endl;
        throw json_exception {&"carattere non identificato: "[ curr_char]};
    }
    return token;///se non è nessuna di queste cose ritorna lo stesso il token errato
}


json get_json_string(Token &t) {
    json j = json();
    j.set_string(t.value);
    return j;
}

json get_json_number(Token& t){
    json j = json();
    //std::stoi(t.value);
    j.set_number(std::stod(t.value));///converto la stringa/token t.value in numero?
    return j;
}

json get_json_boolean(Token& t){
    json j = json();
    if(t.value == "True")
        j.set_bool(true);
    else
        j.set_bool(false);

    return j;
}

json get_json_null(Token& t){
    json j = json();
    j.set_null();
    return j;
}

json get_json_list(Tokenizer& t);

json get_json_dictionary(Tokenizer& t){
    json j = json();
    j.set_dictionary();
    Token token = t.get_token();
    std::string sinistra = "";
    bool b = false;
    bool primo = true;
    bool virgola = false;
    while(token.type != TOKEN::GRAFFA_CHIUSA){
        if(token.type == TOKEN::STRING){
            if(sinistra.empty() && !b){
                sinistra = token.value;
            }
            else if(!sinistra.empty() && b){
                if(primo && !virgola) {
                    j.insert({sinistra, get_json_string(token)});
                    b = false;
                    primo = false;
                }
                else if(!primo && virgola){
                    j.insert({sinistra, get_json_string(token)});
                    b = false;
                    virgola = false;
                }
                else{
                    std::cout<< "riga 1444" <<std::endl;
                    throw json_exception{"problema virgola"};
                }

            }
            else{
                std::cout << "riga 1450" << std::endl;
                throw json_exception{"due punti mancanti"};
            }

        }
        else if(token.type == TOKEN::NUMBER){
            if(sinistra.empty() && !b){
                sinistra = token.value;
            }
            else if(!sinistra.empty() && b){
                if(primo && !virgola) {
                    j.insert({sinistra, get_json_number(token)});
                    b = false;
                    primo = false;
                }
                else if(!primo && virgola){
                    j.insert({sinistra, get_json_number(token)});
                    b = false;
                    virgola = false;
                }
                else{
                    std::cout<< "riga 1471" <<std::endl;
                    throw json_exception{"problema virgola"};
                }

            }
            else{
                std::cout << "riga 1477" << std::endl;
                throw json_exception{"due punti mancanti"};
            }
        }
        else if(token.type == TOKEN::BOOLEAN){
            if(sinistra.empty() && !b){
                sinistra = token.value;
            }
            else if(!sinistra.empty() && b){
                if(primo && !virgola) {
                    j.insert({sinistra, get_json_boolean(token)});
                    b = false;
                    primo = false;
                }
                else if(!primo && virgola){
                    j.insert({sinistra, get_json_boolean(token)});
                    b = false;
                    virgola = false;
                }
                else{
                    std::cout<< "riga 1497" <<std::endl;
                    throw json_exception{"problema virgola"};
                }

            }
            else{
                std::cout << "riga 1503" << std::endl;
                throw json_exception{"due punti mancanti"};
            }
        }
        else if(token.type == TOKEN::NULLO){
            if(sinistra.empty() && !b){
                sinistra = token.value;
            }
            else if(!sinistra.empty() && b){
                if(primo && !virgola) {
                    j.insert({sinistra, get_json_null(token)});
                    b = false;
                    primo = false;
                }
                else if(!primo && virgola){
                    j.insert({sinistra, get_json_null(token)});
                    b = false;
                    virgola = false;
                }
                else{
                    std::cout<< "riga 1523" <<std::endl;
                    throw json_exception{"problema virgola"};
                }

            }
            else{
                std::cout << "riga 1529" << std::endl;
                throw json_exception{"due punti mancanti"};
            }
        }
        else if(token.type == TOKEN::GRAFFA_APERTA){
            if(sinistra.empty() && !b){
                sinistra = token.value;
            }
            else if(!sinistra.empty() && b){
                if(primo && !virgola) {
                    j.insert({sinistra, get_json_dictionary(t)});
                    b = false;
                    primo = false;
                }
                else if(!primo && virgola){
                    j.insert({sinistra, get_json_dictionary(t)});
                    b = false;
                    virgola = false;
                }
                else{
                    std::cout<< "riga 1549" <<std::endl;
                    throw json_exception{"problema virgola"};
                }

            }
            else{
                std::cout << "riga 1555" << std::endl;
                throw json_exception{"due punti mancanti"};
            }
        }
        else if(token.type == TOKEN::QUADRA_APERTA){
            //primo = true;
            if(sinistra.empty() && !b){
                sinistra = token.value;
            }
            else if(!sinistra.empty() && b){
                if(primo && !virgola) {
                    j.insert({sinistra, get_json_list(t)});
                    b = false;
                    primo = false;
                }
                else if(!primo && virgola){
                    j.insert({sinistra, get_json_list(t)});
                    b = false;
                    virgola = false;
                }
                else{
                    std::cout<< "riga 1575" <<std::endl;
                    throw json_exception{"problema virgola"};
                }

            }
            else{
                std::cout << "riga 1581" << std::endl;
                throw json_exception{"due punti mancanti"};
            }
        }
        else if(token.type == TOKEN::DUE_PUNTI){
            if(!sinistra.empty() && !b){
                b = true;
            }
            else{
                std::cout << "riga 1590" << std::endl;
                throw json_exception{"formato json non valido"};
            }
        }
        else if(token.type == TOKEN::VIRGOLA){
            if(!virgola){
                virgola = true;
                b = false;
                sinistra = "";
            }
            else{
                std::cout << "riga 1599"<< std::endl;
                throw json_exception{"hai ricevuto una virgola non necessaria"};
            }

        }
        else{
            std::cout << "riga 1605" << std::endl;
            throw json_exception {"formato json non valido"};
        }
        token = t.get_token();
    }
    return j;
}

json get_json_list(Tokenizer& t){
    json j = json();
    j.set_list();
    Token token = t.get_token();
    bool virgola = false;
    bool primo = true;
    while(token.type != TOKEN::QUADRA_CHIUSA) {
        if (token.type == TOKEN::STRING) {
            if(primo && !virgola) {
                j.push_back(get_json_string(token));
                primo = false;
            }
            else if(!primo && virgola){
                j.push_back(get_json_string(token));
                virgola = false;
            }
            else{
                std::cout<< "riga 1630" <<std::endl;
                throw json_exception{"problema virgola"};
            }
        } else if (token.type == TOKEN::NUMBER) {
            if(primo && !virgola) {
                j.push_back(get_json_number(token));
                primo = false;
            }
            else if(!primo && virgola){
                j.push_back(get_json_number(token));
                virgola = false;
            }
            else{
                std::cout<< "riga 1643" <<std::endl;
                throw json_exception{"problema virgola"};
            }
        } else if (token.type == TOKEN::BOOLEAN) {
            if(primo && !virgola) {
                j.push_back(get_json_boolean(token));
                primo = false;
            }
            else if(!primo && virgola){
                j.push_back(get_json_boolean(token));
                virgola = false;
            }
            else{
                std::cout<< "riga 1656" <<std::endl;
                throw json_exception{"problema virgola"};
            }
        } else if (token.type == TOKEN::NULLO) {
            if(primo && !virgola) {
                j.push_back(get_json_null(token));
                primo = false;
            }
            else if(!primo && virgola){
                j.push_back(get_json_null(token));
                virgola = false;
            }
            else{
                std::cout<< "riga 1669" <<std::endl;
                throw json_exception{"problema virgola"};
            }

        } else if (token.type == TOKEN::GRAFFA_APERTA) {
            if(primo && !virgola) {
                j.push_back(get_json_dictionary(t));
                primo = false;
            }
            else if(!primo && virgola){
                j.push_back(get_json_dictionary(t));
                virgola = false;
            }
            else{
                std::cout<< "riga 1683" <<std::endl;
                throw json_exception{"problema virgola"};
            }

        } else if (token.type == TOKEN::QUADRA_APERTA) {
            if(primo && !virgola) {
                j.push_back(get_json_list(t));
                primo = false;
            }
            else if(!primo && virgola){
                j.push_back(get_json_list(t));
                virgola = false;
            }
            else{
                std::cout<< "riga 1697" <<std::endl;
                throw json_exception{"problema virgola"};
            }

        }
        else if(token.type == TOKEN::VIRGOLA){
            if(!virgola){
                virgola = true;
            }
            else{
                std::cout << "riga 1707"<< std::endl;
                throw json_exception{"hai ricevuto una virgola non necessaria"};
            }

        }
        else{
            std::cout << "riga 1713 " << token.value<<std::endl;
            throw json_exception {"formato json non valido"};
        }
        token = t.get_token();
    }
    return j;
}


std::istream &operator>>(std::istream &lhs, json &rhs) {
    //std::string st;
    //st.assign(std::istreambuf_iterator<char>(lhs), std::istreambuf_iterator<char>());
    //std::cout << st;
    Tokenizer t = Tokenizer(lhs);


    while (true) {
        Token token = t.get_token();
        if (token.type == TOKEN::GRAFFA_APERTA) {
            rhs = get_json_dictionary(t);

        } else if (token.type == TOKEN::STRING) {
            rhs = get_json_string(token);

        } else if (token.type == TOKEN::NUMBER) {
            rhs = get_json_number(token);

        } else if (token.type == TOKEN::QUADRA_APERTA) {
            rhs = get_json_list(t);

        } else if (token.type == TOKEN::BOOLEAN) {
            rhs = get_json_boolean(token);

        } else if (token.type == TOKEN::NULLO) {
            rhs = get_json_null(token);

        }
        else if(token.type == TOKEN::FINE_INPUT){
            break;
        }
        else{
            std::cout << "riga 1749" << std::endl;
            throw json_exception {"formato json non valiod"};
        }
    }
    return lhs;
}

int main() {
    //here you can test the parser

    return 0;
}


