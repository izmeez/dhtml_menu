$Id$

DESCRIPTION
-----------

DHTML menus uses javascript DHTML to reduce the number of page loads when using
nested menus; this is particularly useful with Drupal's administration system.

Ordinarily in Drupal, when you click on a menu with sub-items, you need to
go to that page and do a page refresh to get the items that are beneath it.
With DHTML Menus, instead the sub-items are expanded as soon as you click
on it. Additionally, it uses a cookie to remember what menus are open and
what menus are closed, so as you navigate around the site your menus remain
consistent.


INSTALLATION
------------

1) Drop the entire directory into your modules directory or sites/all/modules
   directory.
2) Enable the module at administer >> site building >> modules
3) All menu blocks will be converted automatically to DHTML menus.
4) Go to administer >> block. All menu blocks will have an extra option to
   enable/disable this feature.
5) To enable general settings, go to administer >> settings >> DHTML menu.
6) When you uninstall it, all menus will be back to normal automatically

Enjoy!


CREDITS
-------

Arancaytar                     <http://drupal.org/user/28680>
Bruno Massa - "brmassa"        <http://drupal.org/user/67164>
Earl Miles  - "merlinofchaos"  <http://drupal.org/user/26979>
