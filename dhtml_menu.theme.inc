<?php
// $Id$

/**
 * @file dhtml_menu.theme.inc
 * All functions related to generating the menu markup.
 */

/**
 * Preprocessor for menu_item_link.
 * Adds an ID attribute to menu links and helps the module
 * follow the recursion of menu_tree_output().
 */
function dhtml_menu_theme_menu_item_link($link) {
  global $theme;
  static $function;
  $settings = variable_get('dhtml_menu_settings');
  if (!isset($function)) {
    $registry = variable_get('dhtml_menu_theme', array());
    $function = isset($registry[$theme]) && drupal_function_exists($registry[$theme]['menu_item_link']) ? $registry[$theme]['menu_item_link'] : 'theme_menu_item_link';
  }

  // Do not stack items that have no menu or mlid.
  if (empty($link['menu_name']) || empty($link['mlid'])) {
    return $function($link);
  }

  $extended_link = $link;

  // If the menu is blacklisted, mark the link as disabled for DHTML.
  $extended_link['dhtml_disabled'] = !empty($settings['filter']['list'][$link['menu_name']]);

  // Add the ID attribute.
  $extended_link = array_merge_recursive($extended_link, array('localized_options' => array('attributes' => array())));
  $extended_link['localized_options']['attributes']['id'] = 'dhtml_menu-' . _dhtml_menu_unique_id($link['mlid']);

  // Each link in series is another level of recursion. Add it to the stack, even if it is disabled.
  _dhtml_menu_stack($extended_link);

  // Pass the altered variables to the normal menu themer, but only if DHTML should be used.
  return $function(!$extended_link['dhtml_disabled'] ? $extended_link : $link);
}

/**
 * Preprocessor for menu_item.
 * Checks whether the current item has children that
 * were not rendered, and loads and renders them.
 */
function dhtml_menu_theme_menu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
  global $theme;
  static $cookie, $function;
  if (!isset($cookie)) {
    $cookie = explode(',', @$_COOKIE['dhtml_menu']);
    $registry = variable_get('dhtml_menu_theme', array());
    $function = isset($registry[$theme]) && drupal_function_exists($registry[$theme]['menu_item']) ? $registry[$theme]['menu_item'] : 'theme_menu_item';
  }

  /* When theme('menu_item') is called, the menu tree below it has been
   * rendered already. Since we are done on this recursion level,
   * one element must be popped off the stack.
   */
  $stack = _dhtml_menu_stack();

  // Move to the last element in the stack (the current item).
  end($stack);
  
  // If this item should not have DHTML, then return to the "parent" function.
  $current = current($stack);
  if (!$current || !empty($current['dhtml_disabled'])) {
    return $function($link, $has_children, $menu, $in_active_trail, $extra_class);
  }

  $extra_class .= ' dhtml-menu ';

  // If there are children, but they were not loaded...
  if ($has_children && !$menu) {
    // Load the tree below the current position.
    $tree = _dhtml_menu_subtree($stack);

    // Render it...
    $menu = menu_tree_output($tree);
    if (!$menu) $has_children = FALSE; // Sanitize tree.
  }

  // If the current item can expand, and is neither saved as open nor in the active trail, close it.
  if ($menu && !($in_active_trail || in_array(substr(key($stack), 5), $cookie))) {
    $extra_class .= ' collapsed start-collapsed ';
  }

  // Cascade up to the original theming function.
  return $function($link, $has_children, $menu, $in_active_trail, $extra_class);
}

/**
 * Helper function for storing recursion levels.
 *
 * @param $link
 *   If a menu item link is passed, it will be appended to the stack.
 *   If none is given, the stack will be returned and popped by one.
 *
 * @return
 *   The stack, if no parameter is given.
 */
function _dhtml_menu_stack($link = FALSE) {
  static $stack = array();
  if ($link) {
    $stack[$link['localized_options']['attributes']['id']] = $link;
  }
  else {
    $copy = $stack;
    array_pop($stack);
    return $copy;
  }
}

/**
 * Traverses the menu tree and returns the sub-tree of the item
 * indicated by the parameter.
 *
 * @param $stack
 *   An array of menu item links that are nested in each other in the tree.
 *
 * @return
 *   The items below the lowest item in the stack.
 */
function _dhtml_menu_subtree($stack) {
  static $index = array();
  static $indexed = array();

  reset($stack);
  $start = current($stack);

  // This looks expensive, but menu_tree_all_data uses static caching.
  $tree = menu_tree_all_data($start['menu_name']);

  if (!isset($indexed[$start['menu_name']])) {
    $index += _dhtml_menu_index($tree);
    $indexed[$start['menu_name']] = TRUE;
  }

  // Traverse the tree.
  foreach ($stack as $item) {
    $key = $index[$item['mlid']];
    if (!isset($tree[$key])) {
      $tree = $tree[key($tree)]['below'];
      if (!isset($tree[$key])) return array();
    }
    $tree = $tree[$key]['below'];
  }
  return $tree;
}

/**
 * Indexes the menu tree by mlid. This is needed to identify the items
 * without relying on titles. This function is recursive.
 *
 * @param $tree
 *   A tree of menu items such as the return value of menu_tree_all_data()
 *
 * @return
 *   An array associating mlid values with the internal keys of the menu tree.
 */
function _dhtml_menu_index($tree) {
  $index = array();
  foreach ($tree as $key => $item) {
    $index[$item['link']['mlid']] = $key;
    if (!empty($item['below'])) {
      $index += _dhtml_menu_index($item['below']);
    }
  }
  return $index;
}

/**
 * Keeps track of ID attributes and adds a suffix to make it unique-when necessary.
 */
function _dhtml_menu_unique_id($id) {
  static $ids = array();
  if (!isset($ids[$id])) {
    $ids[$id] = 1;
    return $id;
  }
  else {
    return $id . '-' . $ids[$id]++;
  }
}

