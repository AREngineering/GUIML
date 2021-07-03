#ifndef ITEM_H
#define ITEM_H

#include "GUIML.h"

typedef void (*tItemHandler)(Item*);

#define MAX_ITEMS  16
#define MAX_CHARS  256

class Interface;

// Parent Class from which all the menulib interractive screens classes will be derived
class Item
{
  protected:
	// Label that identifies the object
    char text[MAX_CHARS];
    // Index that indicates what is the next object
    // linked in the pElements array
    uint8_t cursor = 0;
    // Array of all the pointers to the object that
    // can be accessed in the interactive screen represented
    // by this object
    Item *pElements[MAX_ITEMS];
    char *pTextElements[MAX_ITEMS];
    tItemHandler pFHandlers[MAX_ITEMS];
    Item *pParents[MAX_ITEMS];
    // Number of elements in the pElements array
    uint8_t elements_number = 0; 
    // Flag that indicates if this object represents the base screen for the menu
    uint8_t is_home = 0;
    
    // Pointer to the U8G2 object that handles the displaz
    U8G2 *pU8g2;
    // If the object is not the base object this variable
    // contains the pointer to the base object
    Item *pHome;
    // Pointer to the menulib object that called this object
    Item *pParent = NULL;
    // For the base object this pointer links to the current menulib
    // object to be displayed
    Item *pSelected_child;
    // Pointer to the Interface Object that handles the sending and receiving of the various
    // signals
    Interface *pInterface;
    // Variables used for displaying shifting strings
    int16_t  index_position = 0;
    uint8_t  draw_ascending = 0;


    virtual void draw(void) =0;
    void set_parent(Item* pPar){pParent = pPar;};
    void set_selected_child(Item *pElement, Item* pPar=NULL);

	void draw_sliding_string(char *input,int16_t y);
	void update_index_position(void);
	void draw_multline_string(char *input);

	virtual void increase(void);
	virtual void decrease(void);
	virtual void select(void);
	virtual void communicate(Item *pItm){(void) pItm;};


  public: 
    Item(char* label,uint8_t base_flag=0);  // constructor
    virtual ~Item();
    void SetPInterface(Interface *pIntfc);
    void SetU8g2(U8G2 *pObj);
    virtual void AddElement(char* in_str, Item *pEl, tItemHandler pHdlr);
    //virtual void AddElement(Item *pElement);
    void RemoveElement(uint8_t idx);
    void RemoveElement(char* lbl);
    void SetHome(Item *pElement);

    /*
     * @Activate
     * function used by the base object to signal the
     * selected child object to be displayed to use its
     * draw function
     */
    void Activate(void) {pSelected_child->draw();};
    void SendEvent(eventSignal_t ev);

    /*
     * @ReturnHome
     * Signals to the base Object that the control must be
     * return to it.
     */
    void ReturnHome(void){set_selected_child(pHome);};
    //void GoToNextItem(void);
    
    uint8_t    Cursor(){return cursor;};
    uint8_t    ElementsNumber(void){return elements_number;};
    Interface *PInterface(void){return pInterface;};
    void       Text(char* output) {strcpy(output,text);};
    U8G2      *PU8g2(void) {return pU8g2;};
    Item      *PHome(void) {return pHome;};
    Item      *PParent(void) {return pParent;};

    static void Increase(Item* pItm){pItm->increase();};
    static void Decrease(Item* pItm){pItm->decrease();};
    static void Select(Item* pItm){pItm->select();};
    static Interface* CreateTypicalEncoderInterface(Item *pItm);

    /*
     * @Communicate
     * Certain menulib class like SelectValueUnit works directly with another class that is MenuList
     * This method allows the two connected objects to communicate and share informations required
     * to make the SelectValueUnit class to work.
     */
    virtual void Communicate(Item *pItm){(void) pItm;}; //Function that allow a parent object in the menu to communicate with the selected child.
    virtual void Content(char *output) = 0;
    virtual void OnSelection(void){;};

    uint8_t IsHome(void){return is_home;};
};


//Interface* CreateTypicalEncoderInterface(Item *pItm);
#endif
