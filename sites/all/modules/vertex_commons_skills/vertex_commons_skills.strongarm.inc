<?php
/**
 * @file
 * vertex_commons_skills.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function vertex_commons_skills_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'facetapi:block_cache:search_api@skills';
  $strongarm->value = -1;
  $export['facetapi:block_cache:search_api@skills'] = $strongarm;

  return $export;
}
