// $Id$

/**
 * @file dhtml_menu.js
 * The Javascript code for DHTML Menu
 */
 
Drupal.dhtmlMenu = {};

/**
 * Initialize the module's JS functions
 */
Drupal.behaviors.dhtmlMenu = function() {
  var dhtmlRun;
  
  // Do not run this function more than once.
  if (dhtmlRun) {
    return;
  }
  else {
    dhtmlRun = true;
  }

  // Get the settings.
  var effects = Drupal.settings.dhtmlMenu;

  // Get the cookie and mark the saved menus.
  var cookie = Drupal.dhtmlMenu.cookieGet().split(',');
  
  for (i in cookie) {
    $('#menu-' + cookie[i]).parents('li:first').addClass('dhtml-open');
  }

  // Close the menu items that were not saved as open.
  $('li.expanded').not('.dhtml-open')
  .removeClass('expanded').addClass('collapsed').children('ul').css('display', 'none');
  Drupal.dhtmlMenu.cookieSet();

  /* Add jQuery effects and listeners to all menu items.
   * The ~ (sibling) selector is unidirectional and selects 
   * only the latter element, so we must use siblings() to get 
   * back to the link element.
   */
  $('ul.menu a ~ ul').siblings('a').each(function() {
    if (effects.clone) {
      $(this).clone().prependTo($(this).siblings('ul')).wrap('<li class="leaf fake-leaf"></li>');
    }

    if (effects.doubleclick) {
      $(this).dblclick(function(e) {
        window.location = this.href;
      });
    }

    $(this).click(function(e) {
      Drupal.dhtmlMenu.toggleMenu($(this).parent());
      return false;
    });
  });
}

/**
 * Toggles the menu's state between open and closed.
 *
 * @param li
 *   Object. The <li> element that will be expanded or collapsed.
 */
Drupal.dhtmlMenu.toggleMenu = function(li) {
  var effects = Drupal.settings.dhtmlMenu;

  // If the menu is expanded, collapse it.
  if($(li).hasClass('expanded')) {
    if (effects.slide) {
      $(li).children('ul').animate({height: 'hide', opacity: 'hide'}, '1000');
    }
    else $(li).children('ul').css('display', 'none');
    $(li).removeClass('expanded').addClass('collapsed');
  }
  // Otherwise, expand it.
  else {
    if (effects.slide) {
      $(li).children('ul').animate({height: 'show', opacity: 'show'}, '1000');
    }
    else $(li).children('ul').css('display', 'block');
    $(li).removeClass('collapsed').addClass('expanded');

    // If the effect is on, close all sibling menus.
    if (effects.siblings) {
      var id = $(li).children('a:first').attr('id');
      siblings = $('ul.menu li.expanded').not(':has(#' + id + ')').filter(function() {
        return !$(this).parents().is('#' + id);
      });
      if (effects.slide) {
        $(siblings).children('ul').animate({height: 'hide', opacity: 'hide'}, '1000');
      }
      else $(siblings).children('ul').css('display', 'none');
      $(siblings).removeClass('expanded').addClass('collapsed');
    }
  }

  // Save the current state of the menus in the cookie.
  Drupal.dhtmlMenu.cookieSet();
}

/**
 * Reads the dhtml_menu cookie.
 */
Drupal.dhtmlMenu.cookieGet = function() {
  var c = /dhtml_menu=(.*?)(;|$)/.exec(document.cookie);
  if (c) {
    return c[1];
  }
  else return '';
}

/**
 * Saves the dhtml_menu cooki.
 */
Drupal.dhtmlMenu.cookieSet = function() {
  var expanded = new Array();
  $('li.expanded').each(function() {
    expanded.push($(this).children('a:first').attr('id').substr(5));
  });
  document.cookie = 'dhtml_menu=' + expanded.join(',') + ';path=/';
}

