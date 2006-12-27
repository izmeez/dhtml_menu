// $Id$

Drupal.dhtmlMenu = {};

/**
* Attaches the online users autoupdate behaviour to the block content.
*/
Drupal.dhtmlMenu.autoAttach = function() {
//  $('span.submenu').hide();

  var cookievalue = Drupal.dhtmlMenu.getCookie('dhtml_menu');
  if (cookievalue != "") {
    var cookieList = cookievalue.split(",");
    for (var i = 0; i < cookieList.length; i++) {
      $('#'+ cookieList[i]).show();
      $('#menu-' + cookieList[i]).addClass('expanded');
    }
  }

  $('ul.menu li > a').each(function() {
    this.style.display = 'block';
    this.style.paddingLeft = '2em';
    this.style.marginLeft = '-2em';
    this.style.zIndex = 2;
    $(this).click(function(e) {
       id = $(this).parents()[0].id.replace('menu-', '');
       Drupal.dhtmlMenu.switchMenu($('#'+ id)[0], $(this).parents()[0]);
       return false;
    })
    .dblclick(function(e) {
      window.location = this.href;
    });
  });

  $(window).unload(Drupal.dhtmlMenu.saveMenuState);
};

Drupal.dhtmlMenu.switchMenu = function(submenu, parent) {
  if($(parent).is('.expanded')) {
    $(submenu).css('display', 'none');
    $(parent).removeClass('expanded').addClass('collapsed');
  } else {
    $(submenu).css('display', 'block');
    $(parent).removeClass('collapsed').addClass('expanded');
  }
  Drupal.dhtmlMenu.saveMenuState();
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

  document.cookie = "dhtml_menu=" + blocks + ";path=/" ;
}

if (Drupal.jsEnabled) {
  $(document).ready(Drupal.dhtmlMenu.autoAttach);
}