/**
 * $Id$
 * @author: Bruno Massa http://drupal.org/user/67164
 * @file slideshow_creator.js
 * The main Javacript for this module
 */

Drupal.dhtmlMenu = {};

/**
 * Initialize the module's JS functions
 */
Drupal.behaviors.dhtmlMenu = function(context) {
  // Get the Cookie's data
  var cookievalue = Drupal.dhtmlMenu.getCookie();
  if (cookievalue != '') {
    var cookieList = cookievalue.split(',');
    for (var i = 0; i < cookieList.length; i++) {
      $('#'+ cookieList[i]).show();

      $('menu-'+cookieList[i]).removeClass('collapsed').addClass('expanded');
      $(cookieList[i]).css('display', 'block');
    }
  }

  // Add jQuery effects to all menu items
  $('ul.menu li[@class!="leaf"] > a').each(function() {
    if ($(this).parent().children('div.submenu').length > 0) {
      $(this)
      .css({display: 'block', zIndex: 2})
      .dblclick(function(e) {
        window.location = this.href;
      })
      .click(function(e) {
        id = '#' + $(this).parents()[0].id.replace('menu-', '');
        Drupal.dhtmlMenu.switchMenu(id, $(this).parents()[0]);
        return false;
      });
    }
  });

  // Avoid using the saveMenuState function by mistake
  $(window).unload(Drupal.dhtmlMenu.saveMenuState);
}

/**
 * Changes the state of a submenu from open to close.
 *
 * @param submenu
 *   String. The submenu ID (including the "#")
 * @param parent_menu
 *   String. The parent item ID (including the "#")
 */
Drupal.dhtmlMenu.switchMenu = function(submenu, parent_menu) {
  // First, see if the menu is already expanded or collapsed,
  // and perform the opposing effect
  if($(parent_menu).is('.expanded')) {

    // If the user wants the Fading effects, use it,
    // otherwise, simply make the menu disapear
    if (Drupal.settings.dhtmlMenu_useEffects) {
      $(submenu).slideUp('fast');
    }
    else {
      $(submenu).css('display', 'none');
    }

    // Set the parent menu item as collapsed
    $(parent_menu).removeClass('expanded').addClass('collapsed');
  }
  else {
    if (Drupal.settings.dhtmlMenu_useEffects) {
      $(submenu).slideDown('fast');
    }
    else {
      $(submenu).css('display', 'block');
    }
    $(parent_menu).removeClass('collapsed').addClass('expanded');
  }

  // After all changes, save the current state
  // of the menus, so other pages will load and use
  // this new layout
  Drupal.dhtmlMenu.saveMenuState();
}

/**
 * Grabs the cookie data.
 *
 * @return
 *   String. A list, separated by comma, of all menu IDs
 *   that should be expanded.
 */
Drupal.dhtmlMenu.getCookie = function() {
  var string = 'dhtml_menu=';
  var returnvalue = '';

  // If there is a previous instance of this cookie
  if (document.cookie.length > 0) {

    // Get the number of characters that have the list of values
    // from 'dhtml_menu' index.
    offset = document.cookie.indexOf(string);

    // If its positive, there is a list!
    if (offset != -1) {
      offset += string.length;
      var end = document.cookie.indexOf(';', offset);
      if (end == -1) {
        end = document.cookie.length;
      }

      // Get a list of all values that are saved on 'dhtml_menu='
      returnvalue = unescape(document.cookie.substring(offset, end));
    }
  }

  return returnvalue;
}

/**
 * Saves the states of the menus.
 */
Drupal.dhtmlMenu.saveMenuState = function() {
  var menus = '';

  // Get a list of menu IDs, saparated by comma
  $('div.submenu').each(function(i) {
    if (this.style.display != 'none') {
      if (menus != '') {
        menus += ',';
      }
      menus += this.id;
    }
  });

  // Save this values on the cookie
  document.cookie = 'dhtml_menu=' + menus + ';path=/';
}
