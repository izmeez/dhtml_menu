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
  $settings = variable_get('dhtml_menu_settings');
 
  $options['nav'] = array(
    '#type' => 'radios',
    '#title' => t('Static navigation'),
    '#options' => array(
      'pseudo-child' => t('<strong>Fake child item:</strong> At the top of each submenu, an extra link will be generated that leads to the page of the parent item.'),
      'bullet' => t('<strong>Link remains static:</strong> All menu links will continue to function as static links. To expand a menu, click the bullet icon next to the link.'),
      'hover' => t('<strong>Expand on Hover:</strong> Items expand when the cursor hovers over them. Links function normally. <em>This is unfamiliar and causes accessibility problems!</em>'),
      'double-click' => t('<strong>Doubleclick:</strong> To expand a menu, click the link once. To navigate to the page, click it twice. <em>This may be difficult to find for your users!</em>'),
      'none' => t('<strong>None:</strong> Clicking the link will expand the menu. Navigating to the page is not possible. <em>This will make pages with sub-items very difficult to reach!</em>'),
    ),
    '#default_value' => $settings['nav'],
    '#description' => t('Dynamic expansion of menus competes with the static navigation of content. Choose how to resolve this conflict.'),
  );
  
  $options['animation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Animation'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $options['animation']['effects'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Effects'),
    '#options' => array(
      'height' => t('Slide in vertically'),
      'width' => t('Slide in horizontally'),
      'opacity' => t('Fade in'),
    ),
    '#description' => t('You may pick any number of animation effects that will accompany the opening and closing of a menu.'),
    '#default_value' => $settings['animation']['effects'],
  );
  
  $options['animation']['speed'] = array(
    '#type' => 'select',
    '#title' => t('Speed'),
    '#options' => array(100 => 'Very Fast (0.1s)', 500 => 'Fast (0.5s)', 1000 => 'Medium (1s)', 1500 => 'Slow (1.5s)', 2000 => 'Very Slow (2s)'),
    '#default_value' => $settings['animation']['speed'],
    '#description' => t('Choose how quickly the menus should expand and collapse.'),
  );
  
  $options['effects'] = array(
    '#type' => 'fieldset',
    '#title' => t('Other effects'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $options['effects']['siblings'] = array(
    '#type' => 'radios',
    '#title' => t('When a menu opens'),
    '#options' => array(
      'none' => t('Do nothing.'),
      'close-all' => t('Close all other open menus on the page.'),
      'close-same-tree' => t('Close other open submenus in the same menu tree.'),
    ),
    '#default_value' => $settings['effects']['siblings'],
  );
  
  $options['effects']['children'] = array(
    '#type' => 'radios',
    '#title' => t('When a menu closes'),
    '#options' => array(
      'none' => t('Remember which sub-items were expanded when it next opens.'),
      'close-children' => t('Close all its sub-items, too.'),
    ),
    '#default_value' => $settings['effects']['children'],
  );

  $options['filter'] = array();

  $options['filter']['list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Menus without DHTML'),
    '#options' => _dhtml_menu_menus(),
    '#default_value' => $settings['filter']['list'],
    '#description' => t('DHTML will be used for all menus by default, but can be switched off for specific menus.'),
  );

  $form = array(
    '#tree' => TRUE,
    'dhtml_menu_settings' => $options,
  );

  return system_settings_form($form, FALSE);
}

/**
 * Build a human-readable option list for all non-empty menus.
 * Custom menus and book menus are included if the respective modules are enabled.
 */
function _dhtml_menu_menus() {
  if (drupal_function_exists('menu_get_menus')) {
    // If the menu module is enabled, list custom menus.
    $menus = menu_get_menus();
  }
  else {
    // Otherwise, list only system menus.
    $menus = menu_list_system_menus();
  }

  // If the book module is enabled, also include book menus.
  if (drupal_function_exists('book_get_books')) {
    foreach (book_get_books() as $bid => $link) {
      $menus["book-toc-$bid"] = t('Book: %title', array('%title' => $link['title']));
    }
  }

  // @TODO: Critical core bug #473240 does not let us call menu_get_names();
  return $menus;

  // menu_get_names() filters out empty menus, and adds any menus not found using the above calls (edge case).
  foreach (menu_get_names() as $menu) {
    $opts[$menu] = isset($menus[$menu]) ? $menus[$menu] : t('Other menu: %menu', array('%menu' => $menu));
  }

  return $opts;
}

