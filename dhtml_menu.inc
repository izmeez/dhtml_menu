<?php
/**
 * $Id$
 * @file dhtml_menu.inc
 * Allow click expansion of the menu tree via javascript, with
 * graceful degradation.
 */

function _dhtml_menu_build_menu($delta) {
  // Build the menu tree using DHTML menu special
  // construction functions
  return theme('dhtml_menu_tree', menu_tree_all_data($delta));
}

/**
 * Fetch duplicated menu items - this is a configurable setting.
 */
function _dhtml_menu_get_add_links() {
  static $dhtml_menu_duplicated;

  if (!isset($dhtml_menu_duplicated)) {
    $dhtml_menu_duplicated = array();
    $text = variable_get('dhtml_menu_duplicated', "admin\nadmin/build/menu\nnode/add");
    $text = preg_split('/\n/', $text, -1, PREG_SPLIT_NO_EMPTY); // explode returns array(0 => '') for $text == ''

    foreach($text as $line) {
      $line = trim($line);
      preg_match('/^([^ ]+)( (.*))?$/',$line,$match);
      $dhtml_menu_duplicated[$match[1]] = !empty($match[3]) ? $match[3] : TRUE;
    }
  }

  return $dhtml_menu_duplicated;
}

/**
 * Theme each menu item, adding important CSS data
 *
 * @ingroup themable
 */
function theme_dhtml_menu_item($item, $id) {
  static $expanded = NULL;
  if (!is_array($expanded)) {
    if (!isset($_COOKIE['dhtml_menu'])) {
      $_COOKIE['dhtml_menu'] = '';
    }
    $expanded = explode(',', $_COOKIE['dhtml_menu']);
  }

  // Unset hidden sub-items. This avoids items with empty, all-hidden sub-menus.
  if (!empty($item['below'])) {
    foreach ($item['below'] as $index => $leaf) {
      if ($leaf['link']['hidden']) {
        unset($item['below'][$index]);
      }
    }
  }

  // Check if the menu item has a submenu
  // If no, create a normal menu item
  // If yes, create the submenu too
  if (empty($item['below'])) {
    return '  <li class="leaf" id="menu-leaf'. $id .'">' . theme('menu_item_link', $item['link']) . "</li>\n";
  }
  else {
    $type = isset($item['link']['type']) ? $item['link']['type'] : NULL;
    if ($item['link']['in_active_trail'] or $item['link']['expanded'] or in_array("sub$id", $expanded)) {
      $state = 'expanded';
      $display = '';
    }
    else {
      $state = 'collapsed';
      $display = ' style="display: none;"';
    }

    return "<li class=\"menutitle $state\" id=\"menu-sub$id\">"
      . theme('menu_item_link', $item['link'])
      ."<div class=\"submenu\" id=\"sub$id\"$display>"
      . theme('dhtml_menu_tree', $item['below'], $item['link'], $id)
      .'</div>'
      .'</li>';
  }
}

/**
 * Theme each menu, adding important CSS data
 *
 * @ingroup themable
 */
function theme_dhtml_menu_tree($tree, $parent = NULL, $pid = NULL) {
  // If no further items, return blank
  if (empty($tree)) {
    return '';
  }

  // do not include disabled menu items
  if ($item['link']['hidden'] == '1') {
    return NULL;
  }

  $add_links = _dhtml_menu_get_add_links();
  $output = '';
  if ($parent and isset($add_links[$parent['link_path']])) {
    $duplication = $parent;
    if ($add_links[$parent['link_path']] !== TRUE) {
      $duplication['title'] = $add_links[$parent['link_path']];
    }
    $output .= '<li class="leaf" id="menu-leaf'. $pid .'">'. theme('menu_item_link', $duplication) ."</li>\n";
  }

  $class = 'menu';
  if ($parent == NULL) {
    $class .= ' menu-root';

    // Add the propper JS file
    drupal_add_js(drupal_get_path('module', 'dhtml_menu') .'/dhtml_menu.js');

    // If the Effect is on, tell it to JS
    if ($effects = variable_get('dhtml_menu_use_effects', 0)) {
      drupal_add_js(array('dhtmlMenu_useEffects' => 1), 'setting');
    }
    // If Hide Siblings is on, tell it to JS
    if ($effects = variable_get('dhtml_menu_hide_siblings', 0)) {
      drupal_add_js(array('dhtmlMenu_hideSiblings' => 1), 'setting');
    }
  }

  // Create each menu item
  foreach ($tree as $pid => $item) {
    // hidden sub-items are removed already; this line takes care of root items.
    if ($item['link']['hidden']) {
      continue;
    }

    // pid is going to be a DOM identifier, and can't have some characters
    $forbid = array(' ', '(', ')', '{', '}', '[', ']', '\'');
    $pid = str_replace($forbid, '', $pid);

    $output .= theme('dhtml_menu_item', $item, $pid);
  }

  return "\n<ul class=\"$class\">\n". $output ."\n</ul>\n";
}
