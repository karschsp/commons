<?php
/**
 * @file
 * vertex_commons_views.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function vertex_commons_views_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_display_in_quick_links'
  $field_bases['field_display_in_quick_links'] = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'field_display_in_quick_links',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => '0',
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => '0',
        1 => '1',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_boolean',
  );

  return $field_bases;
}
