#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errno.h"
#include "newt.h"

static void * newtvwindow(char * title, char * button1, char * button2, 
		       char * button3, char * message, va_list args) {
    newtComponent b1, b2 = NULL, b3 = NULL, t, f, answer;
    char * buf = NULL;
    int size = 0;
    int i = 0;
    int width, height;
    char * flowedText;
    newtGrid grid, buttonGrid;

    do {
	size += 1000;
	if (buf) free(buf);
	buf = malloc(size);
	i = vsnprintf(buf, size, message, args);
    } while (i == size);

    flowedText = newtReflowText(buf, 35, 5, 5, &width, &height);
    if (height > 6) {
	free(flowedText);
	flowedText = newtReflowText(buf, 60, 5, 5, &width, &height);
    }
    free(buf);

    t = newtTextbox(-1, -1, width, height, NEWT_TEXTBOX_WRAP);
    newtTextboxSetText(t, flowedText);
    free(flowedText);

    if (button3) {
	buttonGrid = newtButtonBar(button1, &b1, button2, &b2, 
				   button3, &b3, NULL);
    } else if (button2) {
	buttonGrid = newtButtonBar(button1, &b1, button2, &b2, NULL);
    } else {
	buttonGrid = newtButtonBar(button1, &b1, NULL);
    }

    newtGridSetField(buttonGrid, 0, 0, NEWT_GRID_COMPONENT, b1, 
		     0, 0, button2 ? 1 : 0, 0, 0, 0);

    grid = newtCreateGrid(1, 2);
    newtGridSetField(grid, 0, 0, NEWT_GRID_COMPONENT, t, 0, 0, 0, 1, 0, 0);
    newtGridSetField(grid, 0, 1, NEWT_GRID_SUBGRID, buttonGrid, 0, 0, 0, 0, 0, 
			NEWT_GRID_FLAG_GROWX);
    newtGridWrappedWindow(grid, title);

    f = newtForm(NULL, NULL, 0);
    newtFormAddComponents(f, t, b1, NULL);

    if (button2)
	newtFormAddComponent(f, b2);
    if (button3)
	newtFormAddComponent(f, b3);

    answer = newtRunForm(f);
    newtGridFree(grid, 1);
 
    newtFormDestroy(f);
    newtPopWindow();

    if (answer == f)
	return NULL;
    else if (answer == b1)
	return button1;
    else if (answer == b2)
	return button2;

    return button3;
}

int newtWinChoice(char * title, char * button1, char * button2, 
		   char * message, ...) {
    va_list args;
    void * rc;

    va_start(args, message);
    rc = newtvwindow(title, button1, button2, NULL, message, args);
    va_end(args);

    if (rc == button1)
	return 0;
    else if (rc == button2)
	return 1;

    return 2;
}

void newtWinMessage(char * title, char * buttonText, char * text, ...) {
    va_list args;

    va_start(args, text);
    newtvwindow(title, buttonText, NULL, NULL, text, args);
    va_end(args);
}

void newtWinMessagev(char * title, char * buttonText, char * text, 
		     va_list argv) {
    newtvwindow(title, buttonText, NULL, NULL, text, argv);
}

int newtWinTernary(char * title, char * button1, char * button2, 
		   char * button3, char * message, ...) {
    va_list args;
    void * rc;

    va_start(args, message);
    rc = newtvwindow(title, button1, button2, button3, message, args);
    va_end(args);

    if (rc == button1)
	return 1;
    else if (rc == button2)
	return 2;
    else if (rc == button3)
	return 3;

    return 0;
}
