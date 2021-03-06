#ifndef EV_EVENT_SYSTEM_H
#define EV_EVENT_SYSTEM_H

#include <gl/gl_declarations.hpp>

///Mouse buttons are 1024th in the list of input
#define MOUSE_BTNS 1024
/// Here has to be 1032 for 1024 keys and 16 mouse buttons
#define INPUT_SIZE (MOUSE_BTNS + 16)

namespace CN
{
	namespace EV
	{
		/// EventSystem singleton class
		/// --Has static callback function for the window
		/// --Keeps tracking of new events
		class CN_API EvSys
		{
		public: // Members
			/// Keep state of every key and mouse button
			/*static bool* keysList;
			/// Keep frame number for each new key and button event
			/// It helps to find whether some key is just pressed
			static UInt* framesList;
			static UInt currFrame;*/

			static double msDeltaX;
			static double msDeltaY;
			static double msPosX;
			static double msPosY;

			static bool cursorBlocked;
			static bool cursorFirst;
		public: // Functions
			/// Define and init keys and frames lists, bind callbacks to window
			static int init(GLFWwindow* wndPtr);
			/// Refresh callbacks, poll events
			/// set mouse deltas to 0, increment frame value
			static void pollEvents();

			// Buttons and keys
			/// Check the list of keys for state of given key
			/// True: the difference between currFrame and event > someValue
			static bool isPressed_key(int keyCode);
			/// If the key in keysList by given code is pressed
			/// And the difference between current frame and event frame
			/// Is lesser than some value - return key state && true
			static bool isJPressed_key(int keyCode);
			/// Check the list of keys for state of given button
			/// True: the difference between currFrame and event > someValue
			static bool isClicked(int btnCode);
			/// If the button in keysList by given code is pressed
			/// And the difference between current frame and event frame
			/// Is lesser than some value - return key state && true
			static bool isJClicked(int btnCode);
			/// Block cursor/bind to the window
			static void toggleCursor(GLFWwindow* wndPtr);

			// --------<Callback Functions>--------
			// Window callback functions
			static void cb_wndClose(GLFWwindow* wndPtr);
			static void cb_wndSize(GLFWwindow* wndPtr, int width, int height);

			// Mouse callback functions
			/// Gets called when the mouse is changed position in a window
			static void cb_mousePos(GLFWwindow* wndPtr,
				double xPos, double yPos);
			/// Gets called when the mouse scrollwheel is changed inside the window
			static void cb_mouseScroll(GLFWwindow* wndPtr,
				double offsetX, double OffsetY);
			/// Gets called whenever any mouse button is pressed inside the window
			static void cb_mouseButton(GLFWwindow* wndPtr,
				int button, int action, int mode);

			// Key callback functions
			/// Gets called whenever any key is pressed inside the window
			static void cb_key(GLFWwindow* wndPtr,
				int key, int scanCode, int action, int mode);

			// Other callbacks
			/// Gets called when any glfw error occurs. Have error id and message
			static void cb_onError(int errId, const char* errMsg);
			// --------</Callback Functions>--------
		};
	}
}
#endif

/*
* Development started: 18.10.2020
* By MichailRis tutorial
*/