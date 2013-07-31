<?php
/**
 * @file
 * vertex_commons_views.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function vertex_commons_views_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-link-field_display_in_featured_docume'
  $field_instances['node-link-field_display_in_featured_docume'] = array(
    'bundle' => 'link',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => '2',
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_display_in_featured_docume',
    'label' => 'Display in featured documents?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => '3',
    ),
  );

  // Exported field_instance: 'node-link-field_display_in_quick_links'
  $field_instances['node-link-field_display_in_quick_links'] = array(
    'bundle' => 'link',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => '3',
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_display_in_quick_links',
    'label' => 'Display in quick links?',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(
        'display_label' => 1,
      ),
      'type' => 'options_onoff',
      'weight' => '2',
    ),
  );

  // Exported field_instance: 'node-link-field_url'
  $field_instances['node-link-field_url'] = array(
    'bundle' => 'link',
    'default_value' => NULL,
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'link',
        'settings' => array(),
        'type' => 'link_default',
        'weight' => '0',
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_url',
    'label' => 'URL',
    'required' => 0,
    'settings' => array(
      'attributes' => array(
        'class' => '',
        'configurable_title' => 0,
        'rel' => '',
        'target' => 'user',
        'title' => '',
      ),
      'display' => array(
        'url_cutoff' => '80',
      ),
      'enable_tokens' => 1,
      'rel_remove' => 'default',
      'title' => 'none',
      'title_maxlength' => '128',
      'title_value' => '',
      'url' => 0,
      'user_register_form' => FALSE,
      'validate_url' => 1,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'link',
      'settings' => array(),
      'type' => 'link_field',
      'weight' => '1',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Display in featured documents?');
  t('Display in quick links?');
  t('URL');

  return $field_instances;
}
