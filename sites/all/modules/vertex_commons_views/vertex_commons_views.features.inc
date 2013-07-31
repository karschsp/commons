<?php
/**
 * @file
 * vertex_commons_views.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vertex_commons_views_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vertex_commons_views_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function vertex_commons_views_node_info() {
  $items = array(
    'link' => array(
      'name' => t('Link'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
