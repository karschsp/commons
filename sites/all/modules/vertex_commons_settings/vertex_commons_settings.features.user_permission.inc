<?php
/**
 * @file
 * vertex_commons_settings.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function vertex_commons_settings_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer menu attributes.
  $permissions['administer menu attributes'] = array(
    'name' => 'administer menu attributes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'menu_attributes',
  );

  return $permissions;
}
