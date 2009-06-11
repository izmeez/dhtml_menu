// $Id$
(function($) {

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

    // Sanitize by removing "expanded" on menus already marked "collapsed". 
    $('li.dhtml-menu.collapsed.expanded').removeClass('expanded');
    var cookie = Drupal.dhtmlMenu.cookieGet();
    for (var i in cookie) {
      // If the cookie was not applied to the HTML code yet, do so now.
      var li = $('#dhtml_menu-' + cookie[i]).parents('li:first');
      if ($(li).hasClass('collapsed')) {
        Drupal.dhtmlMenu.toggleMenu(li, $(li).find('a:first'), $(li).find('ul:first'));
      }
    }
  
    var settings = Drupal.settings.dhtmlMenu;

    if (settings.nav == 'bullet') {
      // Create the markup for the bullet overlay, and the amount to shift it to the right in RTL mode.
      var bullet = $('<a href="#" class="dhtml-menu-icon"></a>');
      var rtl = $('html').attr('dir') == 'rtl' ? Math.ceil($('.menu li').css('margin-right').replace('px', '')) + 1 : 0;
    }
  
    /* Add jQuery effects and listeners to all menu items. */
    $('ul.menu li.dhtml-menu:not(.leaf)').each(function() {
      var li = $(this);
      var link = $(this).find('a:first');
      var ul = $(this).find('ul:first');
      
      if (ul.length) {
        if (settings.nav == 'pseudo-child') {
          // Note: a single long class is used here to avoid matching the .dhtml-menu.leaf selector later on.
          link.clone().prependTo(ul).wrap('<li class="leaf dhtml-menu-fake-leaf"></li>');
        } 
        else if (settings.nav == 'doubleclick') {
          link.dblclick(function(e) {
            window.location = link.attr('href');
          });
        }
        
        if (settings.nav == 'bullet') {
          li.addClass('dhtml-folder');
          var b = bullet.clone().prependTo(link).click(function(e) {
            Drupal.dhtmlMenu.toggleMenu(li, link, ul);
            return false;
          });
          // When using RTL, each overlay must be shifted to the other side of the link text, individually.
          if (rtl) {
            // Shift the overlay right by the width of the text and the distance between text and icon.
            b.css('right', '-' + (Math.ceil(link.css('width').replace('px', '')) + rtl) + 'px');
          }
        }
        else {
          link.click(function(e) {
            Drupal.dhtmlMenu.toggleMenu(li, link, ul);
            return false;
          });
        }
      }
    });

    // When using LTR, all icons can be shifted as one, as the text width is not relevant.
    if (settings.nav == 'bullet' && !rtl) {
      // Shift overlay to the left by the width of the icon and the distance between icon and text.
      var shift = '-' + (Math.ceil(($('.menu li').css('margin-left').replace('px', ''))) + 16) + 'px';
      // Shift the overlay using a negative left-hand offset, and the text using a negative right-hand margin.
      $('.dhtml-menu-icon').css('left', shift).css('margin-right', shift);
    }
  }
}

/**
 * Toggles the menu's state between open and closed.
 *
 * @param li
 *   Object. The <li> element that will be expanded or collapsed.
 * @param link
 *   Object. The <a> element representing the menu link anchor.
 * @param ul
 *   Object. The <ul> element containing the sub-items.
 */
Drupal.dhtmlMenu.toggleMenu = function(li, link, ul) {
  var effects = Drupal.settings.dhtmlMenu.effects;

  // If the menu is expanded, collapse it.
  if(li.hasClass('expanded')) {
    Drupal.dhtmlMenu.animate(ul, 'hide');
   
    // If children are closed automatically, find and close them now.
    if (effects.children == 'close-children') {
      Drupal.dhtmlMenu.animate(li.find('li.expanded').find('ul:first'), 'hide');
      li.find('li.expanded').removeClass('expanded').addClass('collapsed')
    }

    li.removeClass('expanded').addClass('collapsed');
  }

  // Otherwise, expand it.
  else {
    Drupal.dhtmlMenu.animate(ul, 'show');
    li.removeClass('collapsed').addClass('expanded');

    // If the siblings effect is on, close all sibling menus.
    if (effects.siblings != 'none') {
      var id = link.attr('id');
      // Siblings are all open menus that are neither parents nor children of this menu.
      li.find('li').addClass('own-children-temp');

      // If the relativity option is on, select only the siblings that have the same root
      if (effects.siblings == 'close-same-tree') {
        var siblings = li.parent().find('li.expanded').not('.own-children-temp').not(':has(#' + id + ')');
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

      $('.own-children-temp, .sibling-children-temp')
        .removeClass('own-children-temp')
        .removeClass('sibling-children-temp');

      Drupal.dhtmlMenu.animate($(siblings).find('ul:first'), 'hide');
      $(siblings).removeClass('expanded').addClass('collapsed');
    }
  }

  // Save the current state of the menus in the cookie.
  Drupal.dhtmlMenu.cookieSet();
}

Drupal.dhtmlMenu.animate = function(ul, open) {
  var settings = Drupal.settings.dhtmlMenu.animation;

  var effects;
  var animate = 0;

  if (!effects) {
    effects = {};
    for (var i in settings.effects) {
      if (settings.effects[i]) {
        effects[i] = open;
        animate++;
      }
    }
  }
  if (animate) {
    ul.animate(effects, settings.speed * 1);
  }
  else ul.css('display', open == 'show' ? 'block' : 'none'); 
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

})(jQuery);
