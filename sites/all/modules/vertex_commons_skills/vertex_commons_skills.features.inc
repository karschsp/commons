<?php
/**
 * @file
 * vertex_commons_skills.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function vertex_commons_skills_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "facetapi" && $api == "facetapi_defaults") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function vertex_commons_skills_views_api() {
  return array("api" => "3.0");
}

/**
 * Implements hook_rdf_default_mappings().
 */
function vertex_commons_skills_rdf_default_mappings() {
  $schemaorg = array();

  // Exported RDF mapping: experience
  $schemaorg['taxonomy_term']['experience'] = array(
    'rdftype' => array(
      0 => 'skos:Concept',
    ),
    'name' => array(
      'predicates' => array(
        0 => 'rdfs:label',
        1 => 'skos:prefLabel',
      ),
    ),
    'description' => array(
      'predicates' => array(
        0 => 'skos:definition',
      ),
    ),
    'vid' => array(
      'predicates' => array(
        0 => 'skos:inScheme',
      ),
      'type' => 'rel',
    ),
    'parent' => array(
      'predicates' => array(
        0 => 'skos:broader',
      ),
      'type' => 'rel',
    ),
  );

  return $schemaorg;
}

/**
 * Implements hook_default_search_api_index().
 */
function vertex_commons_skills_default_search_api_index() {
  $items = array();
  $items['skills'] = entity_import('search_api_index', '{
    "name" : "Skills",
    "machine_name" : "skills",
    "description" : null,
    "server" : "mysql",
    "item_type" : "user",
    "options" : {
      "index_directly" : 1,
      "cron_limit" : "50",
      "fields" : {
        "uid" : { "type" : "integer" },
        "og_user_node" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "node" },
        "field_name_first" : { "type" : "text" },
        "field_name_last" : { "type" : "text" },
        "field_title" : { "type" : "text" },
        "field_extension" : { "type" : "text" },
        "field_sponsor" : { "type" : "integer", "entity_type" : "user" },
        "field_type" : { "type" : "string" },
        "field_site" : { "type" : "string" },
        "field_skill_and_experience" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "taxonomy_term" },
        "field_sponsor_name" : { "type" : "text" },
        "og_user_node__og_membership__1" : { "type" : "list\\u003Cinteger\\u003E", "entity_type" : "og_membership" },
        "search_api_language" : { "type" : "string" }
      },
      "data_alter_callbacks" : {
        "search_api_alter_role_filter" : {
          "status" : 0,
          "weight" : "-10",
          "settings" : { "default" : "1", "roles" : [] }
        },
        "search_api_alter_add_hierarchy" : { "status" : 0, "weight" : "0", "settings" : { "fields" : [] } },
        "search_api_alter_add_viewed_entity" : { "status" : 0, "weight" : "0", "settings" : { "mode" : "full" } },
        "search_api_alter_add_aggregation" : { "status" : 0, "weight" : "0", "settings" : [] },
        "search_api_alter_add_url" : { "status" : 0, "weight" : "0", "settings" : [] }
      },
      "processors" : {
        "search_api_case_ignore" : {
          "status" : 0,
          "weight" : "0",
          "settings" : { "fields" : {
              "field_name_first" : true,
              "field_name_last" : true,
              "field_title" : true,
              "field_extension" : true,
              "field_sponsor_name" : true
            }
          }
        },
        "search_api_html_filter" : {
          "status" : 0,
          "weight" : "10",
          "settings" : {
            "fields" : {
              "field_name_first" : true,
              "field_name_last" : true,
              "field_title" : true,
              "field_extension" : true,
              "field_sponsor_name" : true
            },
            "title" : 0,
            "alt" : 1,
            "tags" : "h1 = 5\\r\\nh2 = 3\\r\\nh3 = 2\\r\\nstrong = 2\\r\\nb = 2\\r\\nem = 1.5\\r\\nu = 1.5"
          }
        },
        "search_api_tokenizer" : {
          "status" : 0,
          "weight" : "20",
          "settings" : {
            "fields" : {
              "field_name_first" : true,
              "field_name_last" : true,
              "field_title" : true,
              "field_extension" : true,
              "field_sponsor_name" : true
            },
            "spaces" : "[^[:alnum:]]",
            "ignorable" : "[\\u0027]"
          }
        },
        "search_api_stopwords" : {
          "status" : 0,
          "weight" : "30",
          "settings" : {
            "fields" : {
              "field_name_first" : true,
              "field_name_last" : true,
              "field_title" : true,
              "field_extension" : true,
              "field_sponsor_name" : true
            },
            "file" : "",
            "stopwords" : "but\\r\\ndid\\r\\nthe this that those\\r\\netc"
          }
        }
      }
    },
    "enabled" : "1",
    "read_only" : "0",
    "rdf_mapping" : []
  }');
  return $items;
}
