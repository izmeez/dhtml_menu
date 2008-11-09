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
  $settings = unserialize(DHTML_MENU_DEFAULT_SETTINGS);

  $form['dhtml_menu_nav'] = array(
    '#type' => 'radios',
    '#title' => t('Static navigation'),
    '#options' => array(
      'pseudo-child' => t('<strong>Fake child item:</strong> At the top of each submenu, an extra link will be generated that leads to the page of the parent item.'),
      'bullet' => t('<strong>Link remains static:</strong> All menu links will continue to function as static links. To expand a menu, click the bullet icon next to the link.'),
      'double-click' => t('<strong>Doubleclick:</strong> To expand a menu, click the link once. To navigate to the page, click it twice. <em>This may be difficult to find for your users!</em>'),
      'none' => t('<strong>None:</strong> Clicking the link will expand the menu. Navigating to the page is not possible. <em>This will make pages with sub-items very difficult to reach!</em>'),
    ),
    '#default_value' => variable_get('dhtml_menu_nav', $settings['nav']),
    '#description' => t('Dynamic expansion of menus competes with the static navigation of content. Choose how to resolve this conflict.'),
  );
  
  $form['animation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Animation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['animation']['dhtml_menu_animations'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Effects'),
    '#options' => array(
      'height' => t('Slide in vertically'),
      'width' => t('Slide in horizontally'),
      'opacity' => t('Fade in'),
    ),
    '#description' => t('You may pick any number of animation effects that will accompany the opening and closing of a menu.'),
    '#default_value' => variable_get('dhtml_menu_animations', $settings['animations']),
  );
  
  $form['animation']['dhtml_menu_speed'] = array(
    '#type' => 'select',
    '#title' => t('Speed'),
    '#options' => array(100 => 'Very Fast (0.1s)', 500 => 'Fast (0.5s)', 1000 => 'Medium (1s)', 1500 => 'Slow (1.5s)', 2000 => 'Very Slow (2s)'),
    '#default_value' => variable_get('dhtml_menu_speed', $settings['speed']),
    '#description' => t('Choose how quickly the menus should expand and collapse.'),
  );
  
  $form['effects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other effects'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['effects']['dhtml_menu_siblings'] = array(
    '#type' => 'radios',
    '#title' => t('When a menu opens'),
    '#options' => array(
      'none' => t('Do nothing.'),
      'close-all' => t('Close all other open menus on the page.'),
      'close-same-tree' => t('Close other open submenus in the same menu tree.'),
    ),
    '#default_value' => variable_get('dhtml_menu_siblings', $settings['siblings']),
  );
  
  $form['effects']['dhtml_menu_children'] = array(
    '#type' => 'radios',
    '#title' => t('When a menu closes'),
    '#options' => array(
      'none' => t('Remember which sub-items were expanded when it next opens.'),
      'close-children' => t('Close all its sub-items, too.'),
    ),
    '#default_value' => variable_get('dhtml_menu_children', $settings['children']),
  );

  $form['dhtml_menu_disabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus without DHTML'),
    '#options' => dhtml_menu_menus(),
    '#default_value' => variable_get('dhtml_menu_disabled', array()),
    '#description' => t('DHTML will be used for all menus by default, but can be switched off for specific menus.'),
  );

  return system_settings_form($form);
}

/**
 * Build human-readable menu names for book menus.
 * This fetches the title of the book outline's root node. Titles are cached for performance.
 */
function dhtml_menu_menus() {
  $titles = cache_get('dhtml_book_titles');
  $titles = isset($titles->data) ? $titles->data : array();

  $menu_internal = menu_get_names();

  foreach ($menu_internal as $key) {
    if (preg_match('/book-toc-([0-9]+)/', $key, $match)) {
      if (!isset($titles[$match[1]])) {
        $node = node_load($match[1]);
        $titles[$match[1]] = $node->title;
      }
      $menus[$key] = t('Book: %title', array('%title' => $titles[$match[1]]));
    }
    else $menus[$key] = $key;
  }

  cache_set('dhtml_book_titles', $titles);
  return $menus;
}

