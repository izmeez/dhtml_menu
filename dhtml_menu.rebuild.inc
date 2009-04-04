<?php
// $Id$

/**
 * @file dhtml_menu.rebuild.inc
 * Contains functions that will only be called when the cache 
 * is rebuilt, like hook_menu. 
 */

/** 
 * Implementation of hook_theme_registry_alter().
 * Replaces the theme functions for the menu_item functions, and stores the
 * original functions in order to call them when this module is done with preprocessing.
 */
function dhtml_menu_theme_registry_alter(&$theme_registry) {
  global $theme;

  // Back up the existing theme functions.
  $themes = variable_get('dhtml_menu_theme', array());
  $themes[$theme] = array(
    'menu_item' => $theme_registry['menu_item']['function'],
    'menu_item_link' => $theme_registry['menu_item_link']['function'],
  );
  variable_set('dhtml_menu_theme', $themes);

  // Replace them with our own. These will "preprocess" and call the real functions.
  $theme_registry['menu_item']['function'] = 'dhtml_menu_theme_menu_item';
  $theme_registry['menu_item_link']['function'] = 'dhtml_menu_theme_menu_item_link';
}

/**
 * Implementation of hook_menu().
 * Adds a settings page.
 */
function dhtml_menu_menu() {
  $menu['admin/settings/dhtml_menu'] = array(
    'title' => 'DHTML Menu',
    'description' => 'Configure the effects of DHTML Menu',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('dhtml_menu_settings'),
    'access arguments' => array('administer site configuration'),
    'file' => 'dhtml_menu.admin.inc',
  );
  return $menu;
}

