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

    // GrÃ¶ÃŸe der Halde, d. h. Anzahl momentan gespeicherter EintrÃ¤ge
    // liefern.
    uint size (){
        uint s = 0;
        for (Node* n = head; n != nullptr; n = n->sibling)
            ++s;
        return s;
    }

    //Zusammenfassen zweier Bäume B1 und B2 mit Grad k
    //zu einem Baum mit Grad k + 1
    //Wenn die Prior ität des Wurzelknotens von B1 größer als die Priorität des Wurzelknotens
    //von B2 ist, mache B1 zum Nachfolger mit dem größten Grad von B2:
    //Andernfalls mache B2 zum Nachfolger mit dem größten Grad von B1

    Node* hilfsoperation(Node* B1, Node* B2, uint k){
        if(B2->entry->prio < B1->entry->prio){
            B1->sibling = B2;
            B2->parent = B1;
            B2->degree = k + 1;
            return B1;
        }
        else{
            B2->sibling = B1;
            B1->parent = B2;
            B1->degree = k + 1;
            return B2;
        }
    }

    //Vereinigen zweier Halden H1 und H2 zu einer neuen Halde H
    //1 Erstelle einen leeren Zwischenspeicher für bis zu drei Bäume.
    //2 Setze k = 0.
    //3 Solange H1 oder H2 oder der Zwischenspeicher nicht leer sind:
    //1 Wenn der erste (verbleibende) Baum von H1 Grad k besitzt,
    //entnimm ihn aus H1 und füge ihn zum Zwischenspeicher hinzu.
    //2 Entsprechend für H2.
    //3 Wenn der Zwischenspeicher jetzt einen oder drei Bäume enthält,
    //entnimm einen von ihnen und füge ihn am Ende von H an.
    //4 Wenn der Zwischenspeicher jetzt noch zwei Bäume enthält,
    //fasse sie zu einem Baum mit Grad k + 1 zusammen,
    //der als „Übertrag“ für den nächsten Schritt im Zwischenspeicher verbleibt.
    //5 Erhöhe k um 1.
    //Beispiel: H1 mit 22 + 21 + 20
    void merge(BinHeap<P,D> H1, BinHeap<P,D> H2){
        uint k = 0, i = 0;
        Node* H = nullptr;
        Node* B1 = H1.head;
        Node* B2 = H2.head;
        //ein array zum zwischenspeichern von maximalen 3 Bäumen
        Node *array[3] = {nullptr, nullptr, nullptr};
        while(B1 != nullptr || B2 != nullptr || array[0] != nullptr || array[1] != nullptr || array[3] != nullptr){
            if(B1 != nullptr && B1->degree == k){
                array[i] = B1;
                B1 = B1->sibling;
                i++;
            }
            else{
                B1 = nullptr;
            }
            if(B2 != nullptr && B2->degree == k){
                array[i] = B2;
                B2 = B2->sibling;
                i++;
            }
            else{
                B2 = nullptr;
            }
            k++;
        }
    }

    // Neuen Eintrag mit Priorität p und zusätzlichen Daten d erzeugen,
    // zur Halde hinzufügen und zurückliefern.
    //Erzeuge eine temporäre Halde mit einem einzigen Baum mit Grad 0, die das Objekt
    //enthält, und vereinige sie mit der aktuellen Halde.
    Entry* insert (P p, D d){
        Entry* e = new Entry(p, d);
        BinHeap<P,D> H;
        H.head = new Node(e);
        merge(H, *this);
        return e;
    }

    // Eintrag mit minimaler PrioritÃ¤t liefern.
    // (Nullzeiger bei einer leeren Halde.)
    Entry* minimum (){

    }

    // Eintrag mit minimaler PrioritÃ¤t liefern
    // und aus der Halde entfernen (aber nicht freigeben).
    // (Bei einer leeren Halde wirkungslos mit Nullzeiger als Resultatwert.)
    Entry* extractMin (){

    }

    // EnthÃ¤lt die Halde den Eintrag e?
    // Resultatwert false, wenn e ein Nullzeiger ist.
    bool contains (Entry* e){

    }

    // PrioritÃ¤t des Eintrags e auf p Ã¤ndern.
    // Hierbei darf auf keinen Fall ein neues Entry-Objekt entstehen,
    // selbst wenn die Operation intern als Entfernen und Neu-EinfÃ¼gen
    // implementiert wird.
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehÃ¶rt.)
    bool changePrio (Entry* e, P p) {

    }

    // Eintrag e aus der Halde entfernen (aber nicht freigeben).
    // (Wirkungslos mit Resultatwert false, wenn e ein Nullzeiger ist
    // oder e nicht zur aktuellen Halde gehÃ¶rt.)
    //Ändere die Priorität des Objekts quasi auf unendlich.
    // Führe dann die Operation „Entnehmen“ aus.
    bool remove (Entry* e){

    }

    // Inhalt der Halde zu Testzwecken ausgeben.
    void dump (){
        for (Node* n = head; n != nullptr; n = n->sibling)
            std::cout << n->entry->prio << " ";
        std::cout << std::endl;
    }
};
