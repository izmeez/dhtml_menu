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

  $form['dhtml_menu_duplicated'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Duplicated menu items'),
    '#description'    => t('You can specify a list of menu items that should be displayed twice: Once as a parent item that expands and collapses the sub-menu and again as its own child item, which points directly to the page (the parent items still links on double-click). Enter one internal path (like "admin") on each line. To specify a custom link text, put it after the path, separated by a space.'),
    '#rows'           => 4,
    '#default_value'  => variable_get('dhtml_menu_duplicated', 'admin'),
  );

  return system_settings_form($form);
}

/**
 * Replaces the navigation block in all themes by
 * assigning its values to this module's block, then
 * disabling the normal block.
 */
function _dhtml_menu_replace() {
  // Ensure that the block table is up to date.
  _block_rehash();

  // Get all blocks that are enabled
  $res = db_query("SELECT status, weight, region, visibility, pages, title, theme
    FROM {blocks} WHERE module = 'user' AND delta = 1");
  while ($row = db_fetch_array($res)) {
    $navigation_settings[$row['theme']] = $row;
  }

  // Enable the equivalent DHTML menu and
  // disable the original one
  foreach ($navigation_settings as $theme => $row) {
    db_query("UPDATE {blocks} SET status = %d, weight = %d, region = '%s',
      visibility = %d, pages = %d, title = '%s'
      WHERE module = 'dhtml_menu'
      AND delta = 'navigation' AND theme = '%s'", $row);
    db_query("UPDATE {blocks} SET status = 0, region = ''
      WHERE module = 'user' AND delta = 1 AND theme = '%s';", $theme);
  }

  drupal_set_message(t('Your navigation block has been replaced with its DHTML equivalent in all currently enabled themes.'), 'status');

  // go to block administration
  drupal_goto('admin/build/block');
}
