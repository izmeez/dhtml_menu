// $Id$

/**
 * @file dhtml_menu.js
 * The Javascript code for DHTML Menu
 */
 
Drupal.dhtmlMenu = {};

/**
 * Initialize the module's JS functions
 */
Drupal.behaviors.dhtmlMenu = {
  attach: function() {
    // Do not run this function more than once.
    // Check whether this is redundant with D7.
    /*if (Drupal.dhtmlMenu.init) {
      return;
    }
    else {
      Drupal.dhtmlMenu.init = true;
    }*/
  
    $('.collapsed').removeClass('expanded');
    var cookie = Drupal.dhtmlMenu.cookieGet();
    for (var i in cookie) {
      // If the cookie was not applied to the HTML code yet, do so now.
      var li = $('#menu-' + cookie[i]).parents('li:first');
      if ($(li).hasClass('collapsed')) {
        Drupal.dhtmlMenu.toggleMenu(li);
      }
    }
  
    var nav = Drupal.settings.dhtmlMenu.nav;
  
    /* Add jQuery effects and listeners to all menu items.
     * The ~ (sibling) selector is unidirectional and selects 
     * only the latter element, so we must use siblings() to get 
     * back to the link element. 
     */
    $('ul.menu li:not(.leaf)').each(function() {
      if (nav == 'pseudo-child') {
        var ul = $(this).find('ul:first');
        if (ul.length) {
          $(this).find('a:first').clone().prependTo(ul).wrap('<li class="leaf fake-leaf"></li>');
        }
      }
  
      if (nav == 'doubleclick') {
        $(this).dblclick(function(e) {
          window.location = $(this).find('a:first').attr('href');
          e.stopPropagation();
        });
      }
  
      $(this).click(function(e) {
        Drupal.dhtmlMenu.toggleMenu(this);
        e.stopPropagation();
        return false;
      });
    });
    
    if (nav == 'bullet') {
      $('ul.menu a').click(function(e) {
        e.stopPropagation();
        return true;
      });
    } else {
      $('ul.menu li.leaf a').click(function(e) {
        e.stopPropagation();
        return true;
      });
    }
  }
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
    Drupal.dhtmlMenu.animate($(li).find('ul:first'), 'hide');
   
    // If children are closed automatically, find and close them now.
    if (effects.children == 'close-children') {
      Drupal.dhtmlMenu.animate($(li).find('li.expanded').find('ul:first'), 'hide');
      $(li).find('li.expanded').removeClass('expanded').addClass('collapsed')
    }

    $(li).removeClass('expanded').addClass('collapsed');
  }

  // Otherwise, expand it.
  else {
    Drupal.dhtmlMenu.animate($(li).find('ul:first'), 'show');
    $(li).removeClass('collapsed').addClass('expanded');

    // If the siblings effect is on, close all sibling menus.
    if (effects.siblings != 'none') {
      var id = $(li).find('a:first').attr('id');

      // Siblings are all open menus that are neither parents nor children of this menu.
      $(li).find('li').addClass('own-children-temp');

      // If the relativity option is on, select only the siblings that have the same root
      if (effects.siblings == 'close-same-tree') {
        var siblings = $(li).parent().find('li.expanded').not('.own-children-temp').not(':has(#' + id + ')');
      }
      // Otherwise, select all menus
      else {
        var siblings = $('ul.menu li.expanded').not('.own-children-temp').not(':has(#' + id + ')');
      }

      // If children should not get closed automatically...
      if (effects.children == 'none') {
        // Remove items that are currently hidden from view (do not close these).
        $('li.collapsed li.expanded').addClass('sibling-children-temp');
        // Only close the top-most open sibling, not its children.
        $(siblings).find('li.expanded').addClass('sibling-children-temp');
        siblings = $(siblings).not('.sibling-children-temp');
      }

      $('.own-children-temp, .sibling-children-temp').removeClass('own-children-temp').removeClass('sibling-children-temp');

      Drupal.dhtmlMenu.animate($(siblings).find('ul:first'), 'hide');
      $(siblings).removeClass('expanded').addClass('collapsed');
    }
  }

  // Save the current state of the menus in the cookie.
  Drupal.dhtmlMenu.cookieSet();
}

Drupal.dhtmlMenu.animate = function(ul, open) {
  var settings = Drupal.settings.dhtmlMenu;
  var effects = {};
  
  for (effect in settings.animations) {
    if (eval("settings.animations." + effect)) {
      eval("effects." + effect + " = open");
    }
  }
  
  //alert(effects);
  if (effects) {
    $(ul).animate(effects, settings.speed * 1);
  }
  else $(ul).css('display', open == 'show' ? 'block' : 'none'); 
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

