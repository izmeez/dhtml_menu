<?php
/**
 * $Id$
 * @file dhtml_menu.asmin.inc
 * Allow click expansion of the menu tree via javascript, with
 * graceful degradation.
 */

/**
 * DHTML Menu Settings page.
 *
 * @ingroup form
 */
function _dhtml_menu_settings() {
  $form['dhtml_menu_use_effects'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('jQuery slide effect'),
    '#description'    => t('Menu slides when it opens and closes.'),
    '#default_value'  => variable_get('dhtml_menu_use_effects', 0),
  );
  $form['dhtml_menu_hide_siblings'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Hide Siblings effect'),
    '#description'    => t('Close all other menu items when one opens.'),
    '#default_value'  => variable_get('dhtml_menu_hide_siblings', 0),
  );

  $form['dhtml_menu_duplicated'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Duplicated menu items'),
    '#description'    => t('You can specify a list of menu items that should be displayed twice: Once as a parent item that expands and collapses the sub-menu and again as its own child item, which points directly to the page (the parent items still links on double-click). Enter one internal path (like "admin") on each line. To specify a custom link text, put it after the path, separated by a space.'),
    '#rows'           => 4,
    '#default_value'  => variable_get('dhtml_menu_duplicated', "admin\nadmin/build/menu\nnode/add"),
  );

  return system_settings_form($form);
}
