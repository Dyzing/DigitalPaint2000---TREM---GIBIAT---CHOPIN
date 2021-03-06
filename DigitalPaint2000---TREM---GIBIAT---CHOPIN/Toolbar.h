/*
	Toolbar.h
	Implements the toolbar on the left of the screen
*/
#include <list>
#include <array>
#pragma once

// define class structures

// class to pass on events to the selected tool
struct Tuple {
	int x;
	int y;
	friend Tuple operator*(float a,Tuple t) {
		Tuple tmp;
			tmp.x = t.x * a;
			tmp.y = t.y * a;
			return tmp;
	}

	friend Tuple operator+(Tuple a, Tuple t) {
		Tuple tmp;
		tmp.x = t.x + a.x;
		tmp.y = t.y + a.y;
		return tmp;
	}
};

struct cotes {
	int xA;
	int yA;
	int xB;
	int yB;
	float coefD;
	float b;
};
class ToolEvents {
public:
	static void Start();
	static void End();
	static void Display(int window_width, int window_height);
	static bool Pressed(int button, int state, int x, int y);
	static bool Hover(int x, int y);
	static bool SpecialKey(int key, int x, int y);
	static bool BlockMousePress(int button, int state, int x, int y);
};

// classes for the tools

class Tool_Pen {
public:
	static bool isMouseDown;
	static int mouseLastX;
	static int mouseLastY;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool Hover(int x, int y);
	static bool BlockMousePress(int button, int state, int x, int y);
};

class Tool_Move {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static int endMouseX;
	static int endMouseY;
	static bool isDisplaying;
	static int flickerFrameCount;
	static bool flickerColor;
	static void Start();
	static void End();
	static void Display(int window_width, int window_height);
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
	static bool SpecialKey(int key, int x, int y);
};

class Tool_Fill {
public:
	static void FillCercle(Colour startColour, int x, int y);
	static void Fill(Colour startColour);
	static bool Pressed(int button, int state, int x, int y);
	static bool PressedLCA(int button, int state, int x, int y);
	static void initEdgeTable();
	static void printTable();

};

class Tool_Rect {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
};

class Tool_Selection {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static int posDepX;
	static int posDepY;
	static int posFinX;
	static int posFinY;
	static int departX;
	static int departY;
	static bool firstPickSelect;
	static int polygone_index;
	static int nbpoly;
	static std::list<Tuple> CotesFenetre;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool SelectRectangle(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
	static void End_Selection();
};
class Tool_Circle {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
};

class Tool_Polygone {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static int departX;
	static int departY;
	static bool firstPick;
	static int nIter;
	static Colour bord_color;
	static std::list<cotes> ListeCotes;
	static std::list<Tuple> ListeSommets;
	static std::list<std::list<Tuple>> MultiSommets;
	static bool Pressed(int button, int state, int cx, int cy);
	static bool BlockMousePress(int button, int state, int x, int y);
	static void EndPolygon();
};
class Tool_Bezier {
public:
	static bool isMouseDown;
	static int startMouseX;
	static int startMouseY;
	static int departX;
	static int departY;
	static bool firstPick;
	static bool Select;
	static bool drop;
	static bool BezierEnded;
	static bool duplicate;
	static bool firstBezier;
	static int nIter;
	static int step;
	static int stepi;
	static bool B1;
	static Colour bord_color;
	static bool dejaScale;
	static std::list<cotes> ListeCotes;
	static std::list<Tuple> ListeSommets;
	static std::list<Tuple> ListeSommetsCurve;
	static std::list<std::list<Tuple>> MultiSommets;
	static std::vector<Tuple> polygonControl;
	static bool Pressed(int button, int state, int cx, int cy);
	static void BezierEtienne();
	static void Bezier1();
	static void Bezier2();
	static void showStep();
	static void drawCurve();
	static Tuple Mid(Tuple a, Tuple b);
	static Tuple Bary(int iStep, int step, Tuple a, Tuple b);
	static void EndBezier();
	static void SuppressionControle1();
	static void SuppressionControle2();
	static void DuplicateControle();
	static void Redraw();
	static void DrawCircleAroundControlPoint(int index, Colour c);
	static void Scale(float const& pas);

};
/*
	This class implements the toolbar on the left of the window
*/
class Toolbar {

public:
	
	// the index of the selected tool
	static int selectedButton;

	// buttons for each tool in the menu
	static Button penButton;
	static Button moveButton;
	static Button fillLCAButton;
	static Button fillCircleButton;
	static Button rectButton;
	static Button selectionButton;
	static Button LCAButton;


	/*
		Callback invoked when a tool button is pressed from the toolbar

		@param button - the button that was pressed
	*/
	static void ToolButtonPressed(Button button) {
		ToolEvents::End();
		if (button.text == "Pen") { selectedButton = 0; }
		if (button.text == "Move") { selectedButton = 1; }
		if (button.text == "Fill Circle") { selectedButton = 2 ; }
		if (button.text == "Line") { selectedButton = 3; }
		if (button.text == "Select") { selectedButton = 4; }
		if (button.text == "LCA") { selectedButton = 7; }
		ToolEvents::Start();
	}


	/*
		Initializes the toolbar
	*/
	static void Init() {
		// start with pen tool selected
		selectedButton = 0;
		// create the buttons for the toolbar
		penButton = Button::Create(0, 100, 95, 40, (char *)"Pen", ToolButtonPressed, true);
		moveButton = Button::Create(0, 140, 95, 40, (char *)"Move", ToolButtonPressed, true);
		fillCircleButton = Button::Create(0, 180, 95, 40, (char *)"Fill Circle", ToolButtonPressed, true);
		LCAButton = Button::Create(0, 220, 95, 40, (char*)"LCA", ToolButtonPressed, true);
		rectButton = Button::Create(0, 260, 95, 40, (char *)"Line", ToolButtonPressed, true);
		selectionButton = Button::Create(0, 300, 95, 40, (char *)"Select", ToolButtonPressed, true);
		

	}


	/*
		Displays the toolbar

		@param window_width - the width of the window
		@param window_height - the height of the window
	*/
	static void Display(int window_width, int window_height) {
		// draw the buttons
		penButton.Display(window_width, window_height);
		moveButton.Display(window_width, window_height);
		fillCircleButton.Display(window_width, window_height);
		rectButton.Display(window_width, window_height);
		selectionButton.Display(window_width, window_height);
		LCAButton.Display(window_width, window_height);
		// draw a blue overlay on the selected button
		glColor4f(0.0f, 1.0f, 1.0f, 0.4f);
		glBegin(GL_QUADS);
			int selY = window_height - ( 100 + (selectedButton * 40));
			glVertex2f(0, selY);
			glVertex2f(95, selY);
			glVertex2f(95, selY - 40);
			glVertex2f(0, selY - 40);
		glEnd();
	}


	/*
		Handles mouse press events passing them on to the selected tool

		@param button - Mouse button pressed
		@param state - State of mouse event (down or up)
		@param x - The x coordinate of the mouse when pressed
		@param y - The y coordinate of the mouse when pressed
		@return Has the event been handled?
	*/
	static bool Pressed(int button, int state, int x, int y) {
		if ((selectedButton != 0) && (penButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 1) && (moveButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 2) && (fillCircleButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 3) && (rectButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 4) && (selectionButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 7) && (LCAButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 21) && (LCAButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 22) && (LCAButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 23) && (LCAButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 24) && (LCAButton.Pressed(button, state, x, y))) {
			return true;
		}
		if ((selectedButton != 25) && (LCAButton.Pressed(button, state, x, y))) {
			return true;
		}
		return false;
	}


	/*
		Handles mouse movement events passed to the tool

		@param x - Mouse position x coordinate
		@param y - Mouse position y coordinate
		@return True if the event gets handled
	*/
	static bool Hover(int x, int y) {
		bool output = false;
		if (penButton.Hover(x, y)) {
			output = true;
		}
		if (moveButton.Hover(x, y)) {
			output = true;
		}
		if (fillCircleButton.Hover(x, y)) {
			output = true;
		}
		if (rectButton.Hover(x, y)) {
			output = true;
		}
		if (selectionButton.Hover(x, y)) {
			output = true;
		}
		if (LCAButton.Hover(x, y)) {
			output = true;
		}
		return output;
	}

};


/*
	Passes on start event to the selected tool
*/
void ToolEvents::Start() {
	switch (Toolbar::selectedButton) {
		case 1:
			Tool_Move::Start();
			break;
	}
}


/*
	Passes on end event ot the selected tool
*/
void ToolEvents::End() {
	switch (Toolbar::selectedButton) {
		case 1:
			Tool_Move::End();
			break;
	}
}


/*
	Passes display event onto selected tool

	@param window_width - the width of the window
	@param window_height - the height of the window
*/
void ToolEvents::Display(int window_width, int window_height) {
	switch (Toolbar::selectedButton) {
	case 1:
		Tool_Move::Display(window_width, window_height);
		break;
	}
}


/*
	Passes Mouse pressed event onto the selected tool

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Has the tool handled the event?
*/
bool ToolEvents::Pressed(int button, int state, int x, int y) {
	switch (Toolbar::selectedButton) {
	case 0:
		if (Tool_Pen::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 1:
		if (Tool_Move::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 2:
		if (Tool_Fill::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 3:
		if (Tool_Rect::Pressed(button, state, x, y)) {
			return true;
		}
		break;
	case 4:
		if (Tool_Selection::Pressed(button, state, x, y)) {
			return true;
		}
		Tool_Fill::printTable();
		break;
	case 5:
		if (Tool_Circle::Pressed(button, state, x, y)) {
			return true;
		}
	case 6:
		if (Tool_Polygone::Pressed(button, state, x, y)){
			return true;
		}
		Tool_Fill::printTable();
	case 7:
		if (Tool_Fill::PressedLCA(button, state, x, y)){
			return true;
		}
	case 50:
		if (Tool_Bezier::Pressed(button, state, x, y)) {
			return true;
		}
	case 51:
		if (Tool_Bezier::Pressed(button, state, x, y)) {
			return true;
		}
	case 10:
		if (Tool_Selection::SelectRectangle(button, state, x, y)) {
			return true;
		}
	case 21:
		Tool_Selection::polygone_index = 21;
		if (Tool_Fill::PressedLCA(button, state, x, y)) {
			return true;
		}
	case 22:
		Tool_Selection::polygone_index = 22;
		if (Tool_Fill::PressedLCA(button, state, x, y)) {
			return true;
		}
	case 23:
		Tool_Selection::polygone_index = 23;
		if (Tool_Fill::PressedLCA(button, state, x, y)) {
			return true;
		}
	case 24:
		Tool_Selection::polygone_index = 24;
		if (Tool_Fill::PressedLCA(button, state, x, y)) {
			return true;
		}
	case 25:
		Tool_Selection::polygone_index = 25;
		if (Tool_Fill::PressedLCA(button, state, x, y)) {
			return true;
		}
	}
	return false;
}


/*
	Passes mouse movement events onto the selected tool

	@param x - Mouse position x coordinate
	@param y - Mouse position y coordinate
	@return True if the event gets handled
*/
bool ToolEvents::Hover(int x, int y) {
	switch (Toolbar::selectedButton) {
	case 0:
		if (Tool_Pen::Hover(x, y)) {
			return true;
		}
		break;
	}
	return false;
}


/*
	Passes special key events (arrow keys) on to the selected tool

	@param key - the key that was pressed
	@param x - x position of the mouse
	@param y - y position of the mouse
	@return Has the event been handled?
*/
bool ToolEvents::SpecialKey(int key, int x, int y) {
	switch (Toolbar::selectedButton) {
	case 1:
		if (Tool_Move::SpecialKey(key, x, y)) {
			return true;
		}
		break;
	}
	return false;
}


/*
	Should the selected tool take priority for receiving mouse events

	@param button - Mouse button pressed
	@param state - State of mouse event (down or up)
	@param x - The x coordinate of the mouse when pressed
	@param y - The y coordinate of the mouse when pressed
	@return Should the selected tool take priority for receiving mouse events
*/
bool ToolEvents::BlockMousePress(int button, int state, int x, int y) {
	switch (Toolbar::selectedButton) {
	case 0:
		if (Tool_Pen::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 1:
		if (Tool_Move::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 3:
		if (Tool_Rect::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 4:
		if (Tool_Selection::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	case 5:
		if (Tool_Circle::BlockMousePress(button, state, x, y)) {
			return true;
		}
		break;
	}
	return false;
}