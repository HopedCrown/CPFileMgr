#include <dirent.h>
#include <gint/display.h>
#include <gint/keyboard.h>
#include <justui/jbutton.h>
#include <justui/jfileselect.h>
#include <justui/jlabel.h>
#include <justui/jlayout.h>
#include <justui/jscene.h>
#include <justui/jwidget.h>
#include <stdio.h>

// Include for Battery Percentage!
// #include "../include/battery.hpp"

#define VERSION "0.0.1-alpha.1"

// HH3 Information
// NOLINTBEGIN
HHK_NAME("Classpad File Manager")
HHK_AUTHOR("HopedCrown")
HHK_DESCRIPTION("Small, Basic File Manager via JustUI and Gint")
HHK_VERSION(VERSION)
// NOLINTEND
/*
static bool all_file_filter(struct dirent const *ent)
{
  if (!jfileselect_default_filter(ent))
    return false;
  if (ent->d_type == DT_DIR)
    return true;
  return true;
}

*/

int main(void) {
  // Create the main scene (fullscreen)
  jscene *scene = jscene_create_fullscreen(NULL);
  jlabel *title = jlabel_create("Classpad File Manager v" VERSION, scene);
  // jlabel *battery_pcent = jlabel_create("<<TEMP>>", scene);

  // We will use a main vertical box layout:
  // Top component will be a stack layout holding our tabs.
  // Bottom component will be a horizontal box holding navigation buttons.
  jlayout_set_vbox(scene)->spacing = 0;
  // ==========================================
  // STACKED TABS
  // ==========================================
  // The main area that actually holds the different tabs
  jwidget *stack = jwidget_create(scene);
  jlayout_set_stack(stack);
  // Stretch to take all remaining vertical space
  jwidget_set_stretch(stack, 1, 1, false);

  // ------------- TAB 1: Welcome -------------
  jwidget *tab1 = jwidget_create(stack);
  jlayout_set_vbox(tab1);
  // Stretch to take all remaining vertical space
  jwidget_set_stretch(tab1, 1, 1, false);
  jfileselect *jfs = jfileselect_create(tab1);
  jwidget_set_stretch(jfs, 1, 1, false);
  jfileselect_set_saveas(jfs, true);
  jfileselect_set_show_file_size(jfs, true);
  jfileselect_browse(jfs, "/");

  // ------------- TAB 2: Interactive -------------
  jwidget *tab2 = jwidget_create(stack);
  jlayout_set_vbox(tab2)->spacing = 15;
  jwidget_set_padding(tab2, 15, 15, 15, 15);

  // Create Settings Header
  jlabel_create("Settings", tab2);
  jlabel_create("Settings Option 1", tab2);
  jlabel_create("Settings Option 2", tab2);

  /*


  jlabel_create("Features (Tab 2)", tab2);

  // A button to tap!
  jbutton *btn_action = jbutton_create("Click Me!", tab2);
  // Large padding for touchscreen friendliness
  jwidget_set_padding(btn_action, 15, 20, 15, 20);

  jlabel *t2_status = jlabel_create("Button not clicked yet.", tab2);
  */

  // RIP: TAB 3, The About Screen

  // TAB 4: Options Screen (NOT A TAB DIRECTLY ACCESSIBLE VIA BUTONS!)
  jwidget *opt_screen = jwidget_create(stack);
  jlayout_set_vbox(opt_screen)->spacing = 10;
  jwidget_set_padding(opt_screen, 15, 15, 15, 15);
  jlabel_create("----------- File Options Menu -----------", opt_screen);
  jlabel_create("                                         ", opt_screen);
  jlabel_create("            [1] Execute as *.hh3         ", opt_screen);
  jlabel_create("            [2] Edit                     ", opt_screen);
  jlabel_create("            [3] Rename                   ", opt_screen);
  jlabel_create("            [4] Copy/Paste               ", opt_screen);
  jlabel_create("            [5] Move                     ", opt_screen);
  jlabel_create("            [6] Transfer                 ", opt_screen);
  jlabel_create("            [7] Exit                     ", opt_screen);
  jlabel_create("                                         ", opt_screen);
  jlabel_create("------ Identified File Information ------", opt_screen);
  jlabel_create("<<TEMP>>", opt_screen); // Path Placeholder
  jlabel_create("<<TEMP>>", opt_screen); // File Type Placeholder
  jlabel_create("<<TEMP>>", opt_screen); // File Size Placeholder

  // ==========================================
  // BOTTOM NAVIGATION BAR
  // ==========================================
  jwidget *buttons = jwidget_create(scene);
  jlayout_set_hbox(buttons)->spacing = 6;
  // Don't stretch vertically, stretch horizontally
  jwidget_set_stretch(buttons, 1, 0, false);
  // Touch friendly paddings around the buttons bar
  jwidget_set_padding(buttons, 8, 8, 8, 8);

  jbutton *b_tab1 = jbutton_create("File Selector", buttons);
  jwidget_set_padding(b_tab1, 12, 12, 12, 12);

  jbutton *b_tab2 = jbutton_create("Settings", buttons);
  jwidget_set_padding(b_tab2, 12, 12, 12, 12);
  // Spacer to push the exit button to the right side of the screen
  jwidget *spacer = jwidget_create(buttons);
  jwidget_set_stretch(spacer, 1, 0, false);

  // Crucial: Exit button for touchscreen (ClassPad has no KEY_EXIT)
  jbutton *b_exit = jbutton_create("Exit", buttons);
  jwidget_set_padding(b_exit, 12, 16, 12, 16);

  // ==========================================
  // INITIALIZATION
  // ==========================================
  // Show tab1 by default and disable its button to show it's visually selected
  jscene_show_and_focus(scene, tab1);
  jbutton_set_disabled(b_tab1, true);
  jbutton_set_disabled(b_tab2, false);
  // jbutton_set_disabled(b_tab3, false);

  // Setup Title for Every Tab
  jwidget_set_stretch(title, 1, 0, false);
  jwidget_set_background(title, C_BLACK);
  jlabel_set_text_color(title, C_WHITE);
  jwidget_set_padding(title, 5, 6, 5, 6);

  // Setup Battery Percentage for Every Tab!
  // jwidget_set_stretch(battery_pcent, 1, 0, false);
  // jwidget_set_background(title, C_WHITE);

  // int BatteryVoltage; // Set up battery Voltage Variable for later
  bool running = true;
  // int B_PCENT;
  // const char[8] B_PCENT_STR;

  // ==========================================
  // EVENT LOOP
  // ==========================================
  // int BatteryVoltage;
  while (running) {
    // BatteryVoltage = 100 * Battery_GetVoltage();
    // B_PCENT =

    // jlabel_set_text(battery_pcent, );

    jevent e = jscene_run(scene);

    if (e.type == JSCENE_PAINT) {
      dclear(C_WHITE);
      jscene_render(scene);
      dupdate();
    } else if (e.type == JBUTTON_TRIGGERED) {
      if (e.source == b_exit) {
        running = false;
      } else if (e.source == b_tab1) {
        jscene_show_and_focus(scene, tab1);
        jbutton_set_disabled(b_tab1, true);
        jbutton_set_disabled(b_tab2, false);
        // jbutton_set_disabled(b_tab3, false);
      } else if (e.source == b_tab2) {
        jscene_show_and_focus(scene, tab2);
        jbutton_set_disabled(b_tab1, false);
        jbutton_set_disabled(b_tab2, true);
        //  jbutton_set_disabled(b_tab3, false);
      }
      /*else if (e.source == b_tab3)
      {
        jscene_show_and_focus(scene, tab3);
        jbutton_set_disabled(b_tab1, false);
        jbutton_set_disabled(b_tab2, false);
      //  jbutton_set_disabled(b_tab3, true);
      }
      */
      /*else if (e.source == btn_action)
      {
        action_count++;
        // Use static buffer because jlabel_set_text doesn't copy the string
        // memory
        static char buf[64];
        sprintf(buf, "Clicked %d times!", action_count);
        jlabel_set_text(t2_status, buf);
      }
    }*/
      else if (e.type == JWIDGET_KEY && e.key.type == KEYEV_DOWN) {
        // Hardware fallback for exiting (if available)
        if (e.key.key == KEY_CLEAR || e.key.key == KEY_EXIT ||
            e.key.key == KEY_HOME) {
          running = false;
        }
      } else if (e.type == JFILESELECT_VALIDATED) {
        jscene_show_and_focus(scene, opt_screen);
      }
    }
  }
  jwidget_destroy(scene); // Usually scenes clean up all their children

  return 1;
}
