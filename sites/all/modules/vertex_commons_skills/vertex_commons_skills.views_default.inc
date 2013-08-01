<?php
/**
 * @file
 * vertex_commons_skills.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vertex_commons_skills_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'skills';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_skills';
  $view->human_name = 'Skills';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Skills';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Indexed User: User ID */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  /* Field: Indexed User: First Name */
  $handler->display->display_options['fields']['field_name_first']['id'] = 'field_name_first';
  $handler->display->display_options['fields']['field_name_first']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_name_first']['field'] = 'field_name_first';
  /* Field: Indexed User: Last Name */
  $handler->display->display_options['fields']['field_name_last']['id'] = 'field_name_last';
  $handler->display->display_options['fields']['field_name_last']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_name_last']['field'] = 'field_name_last';
  /* Field: Indexed User: Photo */
  $handler->display->display_options['fields']['field_photo']['id'] = 'field_photo';
  $handler->display->display_options['fields']['field_photo']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_photo']['field'] = 'field_photo';
  $handler->display->display_options['fields']['field_photo']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_photo']['settings'] = array(
    'image_style' => '50x50_avatar',
    'image_link' => '',
  );
  /* Field: Indexed User: Site */
  $handler->display->display_options['fields']['field_site']['id'] = 'field_site';
  $handler->display->display_options['fields']['field_site']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_site']['field'] = 'field_site';
  /* Field: Indexed User: Skill and Experience */
  $handler->display->display_options['fields']['field_skill_and_experience']['id'] = 'field_skill_and_experience';
  $handler->display->display_options['fields']['field_skill_and_experience']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_skill_and_experience']['field'] = 'field_skill_and_experience';
  $handler->display->display_options['fields']['field_skill_and_experience']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['field_skill_and_experience']['view_mode'] = 'full';
  /* Field: Indexed User: Sponsor */
  $handler->display->display_options['fields']['field_sponsor']['id'] = 'field_sponsor';
  $handler->display->display_options['fields']['field_sponsor']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_sponsor']['field'] = 'field_sponsor';
  $handler->display->display_options['fields']['field_sponsor']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['field_sponsor']['view_mode'] = 'full';
  /* Field: Indexed User: Title */
  $handler->display->display_options['fields']['field_title']['id'] = 'field_title';
  $handler->display->display_options['fields']['field_title']['table'] = 'search_api_index_skills';
  $handler->display->display_options['fields']['field_title']['field'] = 'field_title';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'skills';
  $export['skills'] = $view;

  return $export;
}
