// $Id$

Drupal.dhtmlMenu = {};

/**
* Attaches the online users autoupdate behaviour to the block content.
*/
Drupal.dhtmlMenu.autoAttach = function() {
  $('span.submenu').hide();

  var cookievalue = Drupal.dhtmlMenu.getCookie("switchmenu");
  if (cookievalue != "") {
    var cookieList = cookievalue.split(",");
    for (var i = 0; i < cookieList.length; i++) {
      $('#'+ cookieList[i]).show();
      $('#menu-' + cookieList[i]).addClass('expanded');
    }
  }

  $('li.menutitle > a').each(function(i) {
      $(this).click(function(e) {
		parent = $(this).parents()[0];
		id = parent.id.replace('menu-', '');
        Drupal.dhtmlMenu.switchMenu($('#'+ id)[0], parent);
        return false;
      });
  });

  $(window).unload(Drupal.dhtmlMenu.saveMenuState);
};

Drupal.dhtmlMenu.switchMenu = function(submenu, parent) {
  if(submenu.style.display == "none") {
    submenu.style.display = "block";
    $(parent).removeClass('collapsed').addClass('expanded');
  } else {
    submenu.style.display = "none";
    $(parent).removeClass('expanded').addClass('collapsed');
  }
}

Drupal.dhtmlMenu.getCookie = function(Name) {
  var search = Name + "="
  var returnvalue = "";
  if (document.cookie.length > 0) {
    offset = document.cookie.indexOf(search)
    if (offset != -1) {
      offset += search.length
      end = document.cookie.indexOf(";", offset);
      if (end == -1) end = document.cookie.length;
      returnvalue = unescape(document.cookie.substring(offset, end))
    }
  }

  return returnvalue;
}


Drupal.dhtmlMenu.saveMenuState = function() {
  var blocks = "";
  $('span.submenu').each(function(i) {
    if (this.style.display != "none") {
      if (blocks != "") {
        blocks += ",";
      }
      blocks += this.id;
    }
  });

  document.cookie = "switchmenu=" + blocks + ";path=/" ;
}

if (Drupal.jsEnabled) {
  Drupal.dhtmlMenu.autoAttach();
}