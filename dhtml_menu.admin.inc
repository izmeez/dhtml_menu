<?php
// $Id$

/**
 * @file dhtml_menu.admin.inc
 * Functions that are only called on the admin pages.
 */

/**
 * Module settings form.
 */
function dhtml_menu_settings(&$form_state) {
  $form['dhtml_menu_effects'] = array(
    '#type' => 'checkboxes',
    '#title' => t('DHTML Menu effects'),
    '#options' => array(
      'slide' => t('Animated sliding effect'),
      'siblings' => t('Keep only one menu open at a time.'),
      'relativity' => t('For the above option, consider each menu tree separately.'),
      'children' => t('Close menus when their parent menu is closed.'),
      'clone' => t('Add an extra page link to each expanding menu item.'),
      'doubleclick' => t('Go to the page on double-click.'),
    ),
    '#default_value' => variable_get('dhtml_menu_effects', explode(',', DHTML_MENU_DEFAULT)),
  );
  return system_settings_form($form);
}

