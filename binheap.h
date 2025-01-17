// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

// Als Binomial-Halde implementierte Minimum-Vorrangwarteschlange
// mit PrioritÃ¤ten des Typs P und zusÃ¤tzlichen Daten des Typs D.
// An der Stelle, an der BinHeap fÃ¼r einen bestimmten Typ P verwendet
// wird, muss ein Kleineroperator (<) fÃ¼r den Typ P bekannt sein.
// Werte des Typs P dÃ¼rfen nur mit diesem Operator verglichen werden;
// die Ã¼brigen Vergleichsoperatoren (<=, >, >=, ==, !=) dürfen nicht
// verwendet werden.

template <typename P, typename D>
struct BinHeap {
    // Notwendige Vorabdeklaration.
    struct Node;

    // Eintrag einer solchen Warteschlange bzw. Halde, bestehend aus
    // einer PrioritÃ¤t prio mit Typ P und zusÃ¤tzlichen Daten data mit
    // Typ D.
    // Wenn der Eintrag momentan tatsÃ¤chlich zu einer Halde gehÃ¶rt,
    // verweist node auf den zugehÃ¶rigen Knoten eines Binomialbaums
    // dieser Halde; andernfalls ist node ein Nullzeiger.
    struct Entry {
        // PrioritÃ¤t, zusÃ¤tzliche Daten und zugehÃ¶riger Knoten.
        P prio;
        D data;
        Node* node;

        // Initialisierung mit PrioritÃ¤t p und zusÃ¤tzlichen Daten d.
        Entry (P p, D d) : prio(p), data(d), node(nullptr) {}
    };

    // Knoten eines Binomialbaums innerhalb einer solchen Halde.
    // Neben den eigentlichen Knotendaten (degree, parent, child,
    // sibling), enthÃ¤lt der Knoten einen Verweis auf den zugehÃ¶rigen
    // Eintrag.
    struct Node {
        // ZugehÃ¶riger Eintrag.
        Entry* entry;

        // Grad des Knotens.
        uint degree;

        // VorgÃ¤nger (falls vorhanden; Nullzeiger bei einem Wurzelknoten).
        Node* parent;

        // Nachfolger mit dem grÃ¶ÃŸten Grad
        // (falls vorhanden; Nullzeiger bei einem Blattknoten).
        Node* child;

        // ZirkulÃ¤re Verkettung aller Nachfolger eines Knotens
        // bzw. einfache Verkettung aller Wurzelknoten einer Halde,
        // jeweils sortiert nach aufsteigendem Grad.
        Node* sibling;

        // Initialisierung als Wurzelknoten mit Grad 0 und Eintrag e,
        // der dann auf diesen Knoten zurÃ¼ckverweist.
        Node (Entry* e) : entry(e), degree(0),
                          parent(nullptr), child(nullptr), sibling(nullptr) {
            e->node = this;
        }
    };

    // Anfang der Wurzelliste (Nullzeiger bei einer leeren Halde).
    Node* head;

    // Initialisierung als leere Halde.
    BinHeap (): head(nullptr) {}

    // Ist die Halde momentan leer?
    bool isEmpty (){
        return head == nullptr;
    }

    //die Potenz wird hier berechnet
    double potenz(double b,int e){
        if(e==0){
            return 1;
        }
        if(e<0){
            return 1/potenz(b,-e);
        }

        double p2=potenz(b,e/2);
        if(e%2) {
            return p2 * p2 * b;
        }else {
            return p2 * p2;
        }
    }

    // die Anzahl der momentan gespeicherter EintrÃ¤ge
    // liefern
    uint size () {
        uint size = 0;
        if (isEmpty()) {
            return 0;
        }else if(head->child == nullptr && head->sibling == nullptr){
            return 1;
        }else {
            for (Node *n = head; n != nullptr; n = n->sibling) {
                size += potenz(2, n->degree);
            }
        }
        return size;
    }

    // Um einen k+1 Baum zu generieren.
    Node* hilfsoperation(Node* B1, Node* B2){
            if (B2->entry->prio < B1->entry->prio) {
                B2->sibling = nullptr;
                B2->degree = B2->degree + 1;
                B1->parent = B2;
                if (B2->child == nullptr) {
                    B2->child = B1->sibling = B1;
                } else {
                    B1->sibling = B2->child->sibling;
                    B2->child = B2->child->sibling = B1;
                }
                return B2;
            }else{
                B1->sibling = nullptr;
                B1->degree = B1->degree + 1;
                B2->parent = B1;
                if (B1->child == nullptr) {
                    B1->child = B2->sibling = B2;
                } else {
                    B2->sibling = B1->child->sibling;
                    B1->child = B1->child->sibling = B2;
                }
                return B1;
            }
    }

    //Kettet alle übrigen Nodes im Wurzelknoten an
    Node* anketten(Node* B1, Node* B2){
        for(Node* n = B1; n != nullptr; n = n->sibling){
            if(n->sibling != nullptr){
                continue;
            }else{
                n->sibling = B2;
                B2->sibling = nullptr;
                return B1;
            }
        }
        return B1;
    }

    //Merged zwei Biominalheaps
    void Union(BinHeap<P,D> H1, BinHeap<P,D> H2) {
        uint k = 0, anzahlBaum = 0;
        Node *H = nullptr;
        Node *B1 = H1.head;
        Node *B2 = H2.head;
        Node *zwischenspeicher[3] = {nullptr, nullptr, nullptr};
        while (B1 != nullptr || B2 != nullptr || zwischenspeicher[0] != nullptr || zwischenspeicher[1] != nullptr || zwischenspeicher[2] != nullptr) {
            if (B1 != nullptr && B1->degree == k) {
                if (B1->sibling != nullptr && B1->sibling->degree == k) {
                    zwischenspeicher[0] = B1;
                    B1 = B1->sibling;
                    zwischenspeicher[0]->sibling = nullptr;
                    anzahlBaum++;
                }else {
                    zwischenspeicher[0] = B1;
                    anzahlBaum++;
                    zwischenspeicher[0]->sibling = nullptr;
                    B1 = nullptr;
                }
            }
            if (B2 != nullptr && B2->degree == k) {
                if (B2->sibling != nullptr && B2->degree == k) {
                    zwischenspeicher[1] = B2;
                    B2 = B2->sibling;
                    zwischenspeicher[1]->sibling = nullptr;
                    anzahlBaum++;
                }else{
                    zwischenspeicher[1] = B2;
                    zwischenspeicher[1]->sibling = nullptr;
                    anzahlBaum++;
                    B2 = nullptr;
                }
            }
            if (anzahlBaum == 3 || anzahlBaum == 1) {
                if (H == nullptr && zwischenspeicher[0] != nullptr) {
                    H = zwischenspeicher[0];
                }else if (H == nullptr && zwischenspeicher[1] != nullptr){
                    H = zwischenspeicher[1];
                }else if (H == nullptr && zwischenspeicher[2] != nullptr){
                    H = zwischenspeicher[2];
                }else if (H->sibling == nullptr && zwischenspeicher[0] != nullptr) {
                    H->sibling = zwischenspeicher[0];
                }else if (H->sibling == nullptr && zwischenspeicher[1] != nullptr) {
                    H->sibling = zwischenspeicher[1];
                }else if (H->sibling == nullptr && zwischenspeicher[2] != nullptr) {
                    H->sibling = zwischenspeicher[2];
                } else if(H->sibling != nullptr && zwischenspeicher[0] != nullptr) {
                    H = anketten(H, zwischenspeicher[0]);
                }else if(H->sibling != nullptr && zwischenspeicher[1] != nullptr) {
                    H = anketten(H, zwischenspeicher[1]);
                }else if(H->sibling != nullptr && zwischenspeicher[2] != nullptr) {
                    H = anketten(H, zwischenspeicher[2]);
                }
                zwischenspeicher[0] = zwischenspeicher[1] = zwischenspeicher[2] = nullptr;
                anzahlBaum = 0;
                }else if (anzahlBaum == 2) {
                    if (zwischenspeicher[0] != nullptr && zwischenspeicher[1] != nullptr && zwischenspeicher[2] == nullptr) {
                        zwischenspeicher[2] = hilfsoperation(zwischenspeicher[0], zwischenspeicher[1]);
                        zwischenspeicher[0] = zwischenspeicher[1] = nullptr;
                        anzahlBaum = 1;
                    }else if (zwischenspeicher[1] != nullptr && zwischenspeicher[2] != nullptr && zwischenspeicher[2] != nullptr) {
                        zwischenspeicher[0] = hilfsoperation(zwischenspeicher[2], zwischenspeicher[1]);
                        zwischenspeicher[1] = zwischenspeicher[2]= nullptr;
                        anzahlBaum = 1;
                    }
                }

            k++;
        }
        this->head = H;
    }

    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Halde hinzufügen und zurückliefern.
    Entry* insert (P p, D d){
        Entry* e = new Entry(p,d);
        BinHeap<P,D> H1;
        H1.head = new Node(e);
        Union(H1, *this);
        return e;
    }

    // Eintrag mit minimaler Priorität liefern.
    // (Nullzeiger bei einer leeren Halde.)
    Entry* minimum (){
        if (head == nullptr) {
            return nullptr;
        }
        if(head->sibling == nullptr) {
            return head->entry;
        }
        Node* temp = head;
        Entry* min = head->entry;
        while (temp->sibling != nullptr) {
            if (temp->sibling->entry->prio < min->prio) {
                min = temp->sibling->entry;
            }
            temp = temp->sibling;
        }
        return min;
    }

    void searchDel(Node* temp, Node* del){
        Node* temp3 = head;
        BinHeap<P, D> H1;
        BinHeap<P, D> H2;
        while(temp3->sibling != nullptr){
            if(temp3->sibling == del && del->sibling == nullptr){
                temp3->sibling = nullptr;
                if(temp == del->child){
                    temp->parent = nullptr;
                    temp->sibling = nullptr;
                    H1.head = head;
                    H2.head = temp;
                    Union(H2, H1);
                    break;
                }
                if(temp != del->child){
                    SAR(temp, del);
                    break;
                }
            }
            if(temp3->sibling == del && del->sibling != nullptr){
                temp3->sibling = del->sibling;
                del->sibling = nullptr;
                SAR(temp, del);
                break;
            }
            temp3 = temp3->sibling;
        }
    }

    void SAR(Node* temp, Node* del){
        Node* temp2;
        BinHeap<P, D> H1;
        BinHeap<P, D> H2;
        while(temp != del->child){
            temp2 = temp->sibling;
            temp->sibling = nullptr;
            temp->parent = nullptr;
            H1.head = head;
            H2.head = temp;
            Union(H2, H1);
            temp = temp2;
            if(temp == del->child){
                temp->sibling = nullptr;
                temp->parent = nullptr;
                H1.head = head;
                H2.head = temp;
                Union(H2,H1);
                break;
            }
        }
    }

    // Eintrag mit minimaler PrioritÃ¤t liefern
    // und aus der Halde entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMin (){
        Entry* min;
        Node* del;
        Node* temp;
        Node* temp3;
        BinHeap<P, D> H1;
        BinHeap<P, D> H2;
        if(head != nullptr){
            min = minimum();
            del = min->node;
            if(del->child != nullptr){
                temp = del->child->sibling;
                temp3 = head;
                if(temp == del->child){
                    if(head == del && head->sibling == nullptr){
                        head = del->child;
                        head->sibling = nullptr;
                        head->parent = nullptr;
                    }
                    if(head == del && head->sibling != nullptr){
                        head = head->sibling;
                        H1.head = head;
                        H2.head = del->child;
                        Union(H2, H1);
                    }
                    if(head != del){
                        searchDel(temp, del);
                    }
                }
                else{
                    temp3 = head;
                    temp = del->child->sibling;
                    if(head == del &&  head->sibling != nullptr){
                        head = head->sibling;
                        SAR(temp, del);

                    }
                    if(head == del && head->sibling == nullptr){
                        head = nullptr;
                        SAR(temp, del);
                    }
                    if(head != del){
                        searchDel(temp, del);
                    }
                }
            }
            else{
                if(del->sibling != nullptr){
                    head = del->sibling;
                }
                else{
                    head = nullptr;
                }
            }
            return min;
        }
        else{
            return nullptr;
        }
    }

    // Enthält die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){
        if (e == nullptr) {
            return false;
        }
        Node* tmp = head;
        if(e == tmp->entry) {
            return true;
        }else if (e != nullptr){
            while(e->node->parent != nullptr){
                e = e->node->parent->entry;
                if (e->node->parent == nullptr && tmp->sibling == nullptr) {
                    return true;
                }else{
                    while (tmp->sibling != nullptr) {
                        tmp = tmp->sibling;
                        if (e->node == tmp) {
                            return true;
                        }
                    }
                }
            }
        }else{
            return false;
        }
        return false;
    }

    //Tauscht zwei Entrys miteinander,
    // wenn die Priorität des gesuchten kleiner ist.
    Entry* bubleup(Entry* e){
        if (e->node->parent != nullptr) {
            if (e->node->entry->prio < e->node->parent->entry->prio) {
                Entry* temp = e->node->parent->entry;
                e->node->parent->entry = e;
                e->node->entry = temp;
                e->node = e->node->parent;
                return bubleup(e);
            }
        }
        return e;
    }

    // Priorität des Eintrags e auf p ändern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-Einfügen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört.)
    bool changePrio (Entry* e, P p) {
        if (e == nullptr || !contains(e)) {
            return false;
        }
       if(e->node->entry->prio < p){
           if(e->node->child != nullptr){
               D old = e->data;
               remove(e);
               insert(p, old);
               return true;
           }else{
               e->node->entry->prio = p;
               return true;
           }
       }else{
           e->node->entry->prio = p;
           if(e->node->entry->prio < e->node->parent->entry->prio){
               bubleup(e->node->entry);
               return true;
           }
       }
       return false;
    }

    // Eintrag e aus der Halde entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehört.)
    bool remove (Entry* e){
        if (e == nullptr || !contains(e)) {
            return false;
        }
        P p = minimum()->prio;
        while(e->node->parent != nullptr){
            Entry* temp = e->node->parent->entry;
            e->node->parent->entry = e;
            e->node->entry = temp;
            e->node = e->node->parent;
        }
        e->node->entry->prio = p;
        extractMin();
        return true;
    }

    void helpC(Node* node, string s, Node* last){
        if(last != node->sibling && node->sibling->child == nullptr){
            cout << s << node->sibling->entry->prio << " "<< node->sibling->entry->data << endl;
            helpC(node->sibling, s, last);
        }else if (last != node->sibling && node->sibling->child != nullptr){
            cout << s << node->sibling->entry->prio << " "<< node->sibling->entry->data << endl;
            helpC(node->sibling->child, s + "  ", node->sibling->child);
            helpC(node->sibling, s, last);
        }else if (node->sibling == last && node->sibling->child != nullptr){
            cout << s << node->sibling->entry->prio << " " << node->sibling->entry->data << endl;
            helpC(node->sibling->child, s + "  ", node->sibling->child);
        }else if(node->sibling == last && node->child == nullptr){
            cout << s << node->entry->prio << " " << node->entry->data << endl;
        }
    }

    void dump (){
        Node* current = head;
        while(current != nullptr){
            cout << current->entry->prio << " " << current->entry->data << endl;
            if (current->child != nullptr) {
                helpC(current->child, "  ", current->child);
            }
            current = current->sibling;
        }
    }
};