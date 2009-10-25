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
  $function = &drupal_static(__FUNCTION__);
  $settings = variable_get('dhtml_menu_settings');
  if (!isset($function)) {
    $registry = variable_get('dhtml_menu_theme', array());
    if (isset($registry[$theme]) && function_exists($registry[$theme]['menu_item_link'])) {
      $function = $registry[$theme]['menu_item_link'];
    }
    else {
      $function = 'theme_menu_item_link';
    }
  }

  // Do not stack items that have no menu or mlid.
  if (empty($link['menu_name']) || empty($link['mlid'])) {
    return $function($link);
  }

  $extended_link = $link;

  // If the menu is blacklisted or not whitelisted, mark the link as disabled for DHTML.
  $extended_link['dhtml_disabled'] = ($settings['filter']['type'] == 'blacklist') == !empty($settings['filter']['list'][$link['menu_name']]);

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
  $cookie   = &drupal_static(__FUNCTION__ . ':cookie', array());
  $function = &drupal_static(__FUNCTION__ . ':function');

  if (empty($function)) {
    $settings = variable_get('dhtml_menu_settings');
    if ($settings['effects']['remember'] && $settings['nav'] != 'open' && $settings['effects']['siblings'] != 'close-all') {
      $cookie = explode(',', @$_COOKIE['dhtml_menu']);
    }
    $registry = variable_get('dhtml_menu_theme', array());
    if (isset($registry[$theme]) && function_exists($registry[$theme]['menu_item'])) {
      $function = $registry[$theme]['menu_item'];
    }
    else {
      $function = 'theme_menu_item';
    }
  }

  /* When theme('menu_item') is called, the menu tree below it has been
   * rendered already. Since we are done on this recursion level,
   * one element must be popped off the stack.
   */
  $item = _dhtml_menu_stack();

  // If this item should not have DHTML, then return to the "parent" function.
  if (!$item || !empty($item['dhtml_disabled'])) {
    return $function($link, $has_children, $menu, $in_active_trail, $extra_class);
  }

  $extra_class .= ' dhtml-menu ';

  // If there are children, but they were not loaded...
  if ($has_children && !$menu) {
    // Load the tree below the current position.
    $tree = _dhtml_menu_subtree($item);

    // Render it...
    $menu = menu_tree_output($tree);

    // Sanitize the tree - uncheck has_children if no children were loaded.
    if (!$menu) {
      $has_children = FALSE;
    }
  }

  // If the current item can expand, and is neither saved as open nor in the active trail, close it.
  if ($menu && !($in_active_trail || in_array(substr($item['localized_options']['attributes']['id'], 5), $cookie))) {
    $extra_class .= ' collapsed start-collapsed ';
  }

  // Cascade up to the original theming function.
  return $function($link, $has_children, $menu, $in_active_trail, $extra_class);
}

/**
 * Helper function for storing recursion levels.
 *
 * @param $link
 *   If a menu item link is passed, it will be pushed onto the stack.
 *   Otherwise, one element will be popped off the stack.
 *
 * @return
 *   The last element of the stack, if no argument is passed.
 */
function _dhtml_menu_stack($link = FALSE) {
  static $stack = array();
  if ($link) {
    $stack[] = $link;
  }
  else {
    return array_pop($stack);
  }
}

/**
 * Traverses the menu tree and returns the sub-tree of the item
 * indicated by the parameter.
 *
 * @param $item
 *   A menu item link that must contain the keys "mlid" and "menu_name".
 *
 * @return
 *   The tree below the menu item, or an empty array.
 */
function _dhtml_menu_subtree($item) {
  static $index = array();
  static $indexed = array();

  // This looks expensive, but menu_tree_all_data uses static caching.
  $tree = menu_tree_all_data($item['menu_name']);

  // Index the menu tree to find ancestor paths for each item.
  if (!isset($indexed[$item['menu_name']])) {
    $index += _dhtml_menu_index($tree);
    $indexed[$item['menu_name']] = TRUE;
  }

  // If the menu tree does not contain this item, stop.
  if (!isset($index[$item['mlid']])) {
    return array();
  }

  // Traverse the tree using the ancestor path.
  foreach ($index[$item['mlid']]['parents'] as $mlid) {
    $key = $index[$mlid]['key'];
    if (isset($tree[$key])) {
      $tree = $tree[$key]['below'];
    }
    else {
      return array();
    }
  }

  // Go one level further to go below the current item.
  $key = $index[$item['mlid']]['key'];
  return isset($tree[$key]) ? $tree[$key]['below'] : array();
}

/**
 * Indexes the menu tree by mlid. This is needed to identify the items
 * without relying on titles or stacks. This function is recursive.
 *
 * @param $tree
 *   A tree of menu items such as the return value of menu_tree_all_data().
 * @param $ancestors
 *   Optional, used only by internal recursion.
 * @param $parent
 *   Optional, used only by internal recursion.
 *
 * @return
 *   An array associating mlid values with the internal keys of the menu tree,
 *   and all the mlids of the item's ancestors.
 */
function _dhtml_menu_index($tree, $ancestors = array(), $parent = NULL) {
  $index = array();
  if ($parent) {
    $ancestors[] = $parent;
  }

  foreach ($tree as $key => $item) {
    $index[$item['link']['mlid']] = array(
      'key' => $key,
      'parents' => $ancestors,
    );
    if (!empty($item['below'])) {
      $index += _dhtml_menu_index($item['below'], $ancestors, $item['link']['mlid']);
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

