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

  $('.collapsed').removeClass('expanded');

  // Get cookie
  var cookie = Drupal.dhtmlMenu.cookieGet();
  for (var i in cookie) {
    // If the cookie was not applied to the HTML code yet, do so now.
    var li = $('#menu-' + cookie[i]).parents('li:first');
    if ($(li).hasClass('collapsed')) {
      Drupal.dhtmlMenu.toggleMenu(li);
    }
  }

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

    // If children are closed automatically, find and close them now.
    if (effects.children) {
      if (effects.slide) {
        $(li).find('li.expanded').children('ul').animate({height: 'hide', opacity: 'hide'}, '1000');
      }
      else $(li).find('li.expanded').children('ul').css('display', 'none');

      $(li).find('li.expanded').removeClass('expanded').addClass('collapsed')
    }

    $(li).removeClass('expanded').addClass('collapsed');
  }

  // Otherwise, expand it.
  else {
    if (effects.slide) {
      $(li).children('ul').animate({height: 'show', opacity: 'show'}, '1000');
    }
    else $(li).children('ul').css('display', 'block');
    $(li).removeClass('collapsed').addClass('expanded');

    // If the siblings effect is on, close all sibling menus.
    if (effects.siblings) {
      var id = $(li).children('a').attr('id');

      // Siblings are all open menus that are neither parents nor children of this menu.
      $(li).find('li').addClass('own-children-temp');
      siblings = $('ul.menu li.expanded').not('.own-children-temp').not(':has(#' + id + ')');

      // If children should not get closed automatically...
      if (!effects.children) {
        // Remove items that are currently hidden from view (do not close these).
        $('li.collapsed li.expanded').addClass('sibling-children-temp');
        // Only close the top-most open sibling, not its children.
        $(siblings).find('li.expanded').addClass('sibling-children-temp');
        siblings = $(siblings).not('.sibling-children-temp');
      }

      $('.own-children-temp, .sibling-children-temp').removeClass('own-children-temp').removeClass('sibling-children-temp');

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

