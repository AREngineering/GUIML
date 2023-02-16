/*
 * Guiml_ValueUnit.h
 *
 *  Created on: 12 dic 2021
 *      Author: Utente
 */


#include <item.h>
#include <list.h>

/*
 * value_selection_unit
 * Class that implements a GUI page where a value is modified by the user.
 * The value is linked to a global variable (integer or double) via a pointer provided to
 * the constructor.
 */
namespace guiml
{
	class value_selection: public item
	{
		protected:
		// member that indicates what element of the page would be affected by the selection
		uint8_t cursor = 0;
		// member that indicates if the variable is a integer (default) or a double.
		uint8_t int_flag = 1;

		// member that specifies the number of ciphers codying the value of the variable
		uint8_t number_digits = 0;

		// flag that indicates how the activity of the increase decrease methods must operate.
		uint8_t change_cursor = 1; // change_cursor=1 increase/decrease function will change the cursor otherwise it change the cursor digit

		// Member string that contains the illustrative text on the page.
		char *description = NULL;

		// String containing the ciphers encoding the value to set in the linked global variable
		char *digit_string = NULL;

		// Pointers to the global variable to be set by this GUI page.
		int32_t *pIValue = NULL;
		double *pDValue = NULL;

		// Methods operating on the object.
		// increase decrease operate on the cursor member
		void increase(void);
		void decrease(void);

		// Methods that change the cipher under the cursor
		void digit_up(void);
		void digit_down(void);

		// Methods that set the variable and decodes the information in
		// the digits_string
		void select(void);

		// Methods that draws the page.
		virtual void draw(void);

		// auxiliary methods that computes the cursor-position for the
		// ok and back functionality
		uint8_t ok_cursor(void){return number_digits;}
		uint8_t back_cursor(void){return number_digits+1;}

		public:
		// Constructors: one for integer variable, one for float variable.
		value_selection(const char *name, U8G2 *pGraf, uint8_t* fnt, const char *description, uint8_t digits_num, int32_t *pIVal, item* bs=NULL, item* prnt = NULL);
		value_selection(const char *name, U8G2 *pGraf, uint8_t* fnt, const char *description, uint8_t digits_num, double *pVDal,  item* bs=NULL, item* prnt = NULL);
		// Destructor
		~value_selection();


		// static methods to be used in the implementation of the interface.
		static void trigger_increase(item* itm){((value_selection *)itm)->increase();}
		static void trigger_decrease(item* itm){((value_selection *)itm)->decrease();}
		static void trigger_digit_up(item* itm){((value_selection *)itm)->digit_up();}
		static void trigger_digit_down(item* itm){((value_selection *)itm)->digit_down();}
		static void trigger_select(item* itm){((value_selection *)itm)->select();}

		// method called when the ok functionality is activated
		virtual void on_selection(void);

		// method called when the back functionality is activated
		virtual void on_leave(void);

		// method called by the parent object when it receives a forward or backward signal
		virtual void receive_forward(void);
		virtual void receive_backward(void);

		// Method that produces a string containing the value coded in the digit_string
		// used eventually by the parent object to show the content of this object.
		virtual char* get_content(void);

		void read_pIValue();
		void read_pDValue();
	};
}
