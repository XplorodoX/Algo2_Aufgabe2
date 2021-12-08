// Vorzeichenlose ganze Zahl.
using uint = unsigned int;

// Als Binomial-Halde implementierte Minimum-Vorrangwarteschlange
// mit PrioritÃ¤ten des Typs P und zusÃ¤tzlichen Daten des Typs D.
// An der Stelle, an der BinHeap fÃ¼r einen bestimmten Typ P verwendet
// wird, muss ein Kleineroperator (<) fÃ¼r den Typ P bekannt sein.
// Werte des Typs P dÃ¼rfen nur mit diesem Operator verglichen werden;
// die Ã¼brigen Vergleichsoperatoren (<=, >, >=, ==, !=) dÃ¼rfen nicht
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

    double potenz(double b,int e)
    {
        if(e==0) return 1;
        if(e<0) return 1/potenz(b,-e);

        double p2=potenz(b,e/2);
        if(e%2)
            return p2*p2*b;
        else
            return p2*p2;
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

    //Hilfsoperation
    Node* hilfsoperation(Node* B1, Node* B2){
            if (B2->entry->prio <= B1->entry->prio) {
                B2->sibling = nullptr;
                B2->degree = B2->degree + 1;
                B1->parent = B2;
                if (B2->child == nullptr) {
                    B2->child = B1->sibling = B1;
                } else {
                    B1->sibling = B2->child->sibling;
                    B2->child = B2->child->sibling = B1;
                }
            } else if(B1->entry->prio <= B2->entry->prio){
                B1->sibling = nullptr;
                B1->degree = B1->degree + 1;
                B2->parent = B1;
                if (B1->child == nullptr) {
                    B1->child = B2->sibling = B2;
                } else {
                    B2->sibling = B1->child->sibling;
                    B1->child = B1->child->sibling = B2;
                }
            }
            return B2;
    }

    void Union(BinHeap<P,D> H1, BinHeap<P,D> H2) {
        uint k = 0, anzahlBaum = 0;
        Node *H = nullptr;
        Node *B1 = H1.head;
        Node *B2 = H2.head;
        //ein array zum zwischenspeichern von maximalen 3 Bäumen
        Node *zwischenspeicher[3] = {nullptr, nullptr, nullptr};
        while (B1 != nullptr || B2 != nullptr || zwischenspeicher[0] != nullptr || zwischenspeicher[1] != nullptr || zwischenspeicher[2] != nullptr) {
            if (B1 != nullptr && B1->degree == k) {
                if (B1->sibling != nullptr && B1->sibling->degree == k) {
                    zwischenspeicher[0] = B1;
                    B1 = B1->sibling;
                    zwischenspeicher[0]->sibling = nullptr;
                    anzahlBaum++;
                } else {
                    zwischenspeicher[0] = B1;
                    anzahlBaum++;
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
                }else if (H->sibling != nullptr && zwischenspeicher[0] != nullptr){

                }
                else if (H->sibling != nullptr && zwischenspeicher[1] != nullptr) {
                    while(H->sibling != nullptr){
                        if(H->sibling == nullptr){
                            H->sibling = zwischenspeicher[1];
                            break;
                        }else{
                            continue;
                        }
                    }
                }
                else if (H->sibling != nullptr && zwischenspeicher[2] != nullptr) {

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
        delete zwischenspeicher[0], zwischenspeicher[1], zwischenspeicher[2];
        this->head = H;
    }


    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Halde hinzufügen und zurückliefern.
    //Erzeuge eine temporäre Halde mit einem einzigen Baum mit Grad 0, die das Objekt
    //enthält, und vereinige sie mit der aktuellen Halde.
    Entry* insert (P p, D d){
        Entry* e = new Entry(p,d);
        BinHeap<P,D> H1;
        H1.head = new Node(e);
        Union(H1, *this);
        return head->entry;
    }

    // Eintrag mit minimaler PrioritÃ¤t liefern.
    // (Nullzeiger bei einer leeren Halde.)
    Entry* minimum (){
        if (head == nullptr) {
            return nullptr;
        }
        for (Node *n = head; n != nullptr; n = n->sibling) {
            return head->entry;
        }
    }

    // Eintrag mit minimaler PrioritÃ¤t liefern
    // und aus der Halde entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMin (){
        if (head == nullptr) {
            return nullptr;
        }
        Node* tmp = head;
        head = head->sibling;
        return tmp->entry;
    }

    // EnthÃ¤lt die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){
        if (head == nullptr) {
            return false;
        }
        for (Node *n = head; n != nullptr; n = n->sibling) {
            if (n->entry == e) {
                return true;
            }
        }
        return false;
    }

    // PrioritÃ¤t des Eintrags e auf p Ã¤ndern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-EinfÃ¼gen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehÃ¶rt.)
    bool changePrio (Entry* e, P p) {
        if (p <= head->entry->prio) {
            head->entry->prio = p;
            //Wenn die Prior ität des Objekts kleiner als die seines Vorgängers ist:
            //1 Ver tausche die entry-Verweise der beiden Knoten.
            //2 Aktualisiere die zugehörigen Rückverweise der Objekte auf diese Knoten.
            //3 Wiederhole diesen Schritt für den Vorgänger.
            Node* tmp = head;
            Node* tmp2 = head->sibling;
            while (tmp2 != nullptr) {
                if (tmp2->entry->prio < tmp->entry->prio) {
                    tmp->entry = tmp2->entry;
                    tmp2->entry = e;
                    tmp = tmp2;
                    tmp2 = tmp2->sibling;
                }else {
                    tmp = tmp2;
                    tmp2 = tmp2->sibling;
                }
            }
        }else{
            //Ander nfalls, sofer n sich das Objekt nicht in einem Blattknoten befindet:
            //Entfer ne das Objekt und füge es mit der neuen Prior ität wieder ein.

        }
    }

    // Eintrag e aus der Halde entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehÃ¶rt.)
    //Ändere die Priorität des Objekts quasi auf unendlich.
    // Führe dann die Operation „Entnehmen“ aus.
    bool remove (Entry* e){
    }

    // Inhalt der aktuellen Halde muss eingerückt ausgeben werden
    void dump (){

    }
};
