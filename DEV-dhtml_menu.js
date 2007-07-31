// $Id$

dhtmlMenu = {};

/**
 *  Changes the state of a submenu from open to close.
 */
dhtmlMenu.switchMenu = function(submenu, parent) {
  if($(parent).is(".expanded")) {
    if (Drupal.settings.dhtmlMenu.useEffects) {
      $(submenu).slideUp("fast");
    } else {
      $(submenu).css("display", "none");
    }
    $(parent).removeClass("expanded").addClass("collapsed");
  } else {
    if (Drupal.settings.dhtmlMenu.useEffects) {
      $(submenu).slideDown("fast");
    } else {
      $(submenu).css("display", "inline");
    }
    $(parent).removeClass("collapsed").addClass("expanded");
  }
  dhtmlMenu.saveMenuState();
};

/**
 * Grabs the cookie data.
 */
dhtmlMenu.getCookie = function(name) {
  var search = name + "=";
  var returnvalue = "";
  if (document.cookie.length > 0) {
    offset = document.cookie.indexOf(search);
    if (offset != -1) {
      offset += search.length;
      var end = document.cookie.indexOf(";", offset);
      if (end == -1) {
        end = document.cookie.length;
      }
      returnvalue = unescape(document.cookie.substring(offset, end));
    }
  }
  return returnvalue;
};

/**
 * Saves the states of the menus.
 */
dhtmlMenu.saveMenuState = function() {
  var blocks = "";
  $("div.submenu").each(function(i) {
    if (this.style.display != "none") {
      if (blocks != "") {
        blocks += ",";
      }
      blocks += this.id;
    }
  });
  document.cookie = "dhtml_menu=" + blocks + ";path=/";
};

/**
 * Start everything: Attaches the online users autoupdate behaviour
 * to the block content.
 */
$(function() {
  var cookievalue = dhtmlMenu.getCookie("dhtml_menu");
  if (cookievalue != "") {
    var cookieList = cookievalue.split(",");
    for (var i = 0; i < cookieList.length; i++) {
      $("#"+ cookieList[i]).show();
      $("#menu-" + cookieList[i]).addClass("expanded");
    }
  }
  $("ul.dhtml_menu li[@class!=leaf] > a").each(function() {
    if ($(this).parent().children("div.submenu").length > 0) {
      $(this).css({paddingLeft: "2em", marginLeft: "-2em", zIndex: 2}).click(function(e) {
        id = $(this).parents()[0].id.replace("menu-", "");
        dhtmlMenu.switchMenu($("#"+ id)[0], $(this).parents()[0]);
        return false;
      })
      .dblclick(function(e) {
        window.location = this.href;
      });
    }
  });
  $(window).unload(dhtmlMenu.saveMenuState);
});