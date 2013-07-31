<?php
/**
 * @file
 * Drush support for the migrate module
 */

/**
 * Implements hook_drush_command().
 */
function commons_migrate_drush_command() {
  /* $migration_options = array(
    'limit' => 'Limit on the length of each migration process, expressed in seconds or number of items',
    'feedback' => 'Frequency of progress messages, in seconds or items processed',
    'idlist' => 'A comma delimited list of ids to import or rollback. If unspecified, migrate imports all pending items or rolls back all items for the content set.',
    'all' => 'Process all migrations that come after the specified migration. If no value is supplied, all migrations are processed.',
    'instrument' => 'Capture performance information (timer, memory, or all)',
    'force' => 'Force an operation to run, even if all dependencies are not satisfied',
    'group' => 'Name of the migration group to run',
  ); */
  $items['commons-migrate-sponsors'] = array(
    'description' => 'Migrate sponsor data',
    /* 'options' => array(
      'refresh' => 'Recognize new migrations and update counts',
      'group' => 'Name of the migration group to list',
    ), */
  );
  $items['commons-migrate-membership'] = array(
    'description' => 'Migrate team membership',
  );
  $items['commons-migrate-pictures'] = array(
    'description' => 'Migrate employee pictures',
  );
  return $items;
}

function drush_commons_migrate_membership() {
  $db = mssql_connect('mssqlit80:10000', 'people', 'people');
  $people = mssql_select_db('people');

  $query = "SELECT tp.ID, p.personid, p.firstname, p.lastname, p.email, t.Title FROM Team_To_Person tp INNER JOIN person p ON p.personid = tp.PersonID INNER JOIN Team_Main t ON t.ID = tp.TeamID WHERE t.Title NOT LIKE '%Strategy Team%'";

  $result = mssql_query($query);

  while ($row = mssql_fetch_array($result)) {
    $team_name = $row['Title'];
    print $team_name . "\n";
    $query = db_select('node', 'n');
    $query->condition('n.title', $team_name, '=');
    $query->fields('n', array('nid'));
    $res = $query->execute();
    foreach ($res as $record) {
      // print($record->nid) . "\n";
      $gid = $record->nid;
    }

    $user = user_load_by_mail($row['email']);
    if (isset($user->uid) && $user->uid > 0 && isset($gid)) {
      $values = array(
        'entity_type' => 'user',
        'entity' => $user,
        'state' => OG_STATE_ACTIVE,
      );
      og_group('node', $gid, $values);
    }

  }


}

function drush_commons_migrate_sponsors() {
  $db = mssql_connect('mssqlit80:10000', 'people', 'people');
  $people = mssql_select_db('people');

  $query = "SELECT p.personid, p.firstname, p.lastname, p.firstname + '.' + p.lastname AS fullname, S.firstname + ' ' + S.lastname AS sponsor, s.email as sponsoremail, p.email, p.title, p.extension, p.did, p.aboutme, p.location, p.site, p.type, p.remote, p.photo FROM person p join person s ON s.personid=p.supervisorid WHERE p.del IS NULL AND p.type IN ('Employee', 'Contingent Worker') ORDER BY p.lastname ASC;";
  $result = mssql_query($query);
  while ($row = mssql_fetch_array($result)) {
    if ($row['email'] != '') {
      print('EMPLOYEE: ' . $row['email'] . ' - ');
      $account = user_load_by_mail($row['email']);
      print('SPONSOR: ' . $row['sponsor'] . "\n");
      $sponsor = user_load_by_mail($row['sponsoremail']);
      print_r($sponsor);
      // $account->field_sponsor[LANGUAGE_NONE][0]['uid'] = $sponsor->uid;
      $edit = array(
        'field_sponsor' => array(
          'und' => array(
            0 => array(
              'uid' => $sponsor->uid,
            ),
          ),
        ),
      );
      if ($account->name == '') {
        // do nothing
      } else {
        user_save($account, $edit);
      }
    }
  }
}

function drush_commons_migrate_pictures() {
  $db = mssql_connect('mssqlit80:10000', 'people', 'people');
  $people = mssql_select_db('people');

  $query = "SELECT personid, firstname, lastname, email FROM person WHERE type IN ('Employee', 'Contingent Worker') AND del IS NULL";
  $result = mssql_query($query);
  while ($row = mssql_fetch_array($result)) {
    $user = user_load_by_mail($row['email']);
    $filename = 'employee.' . $row['firstname'] . '.' . $row['lastname'] . '.jpg';
    $image = file_get_contents('http://intranet.vertexinc.com/connections/people/getimage.asp?PersonID=' . $row['personid']);
    print "Grabbing image for " . $user->name . "\n";
    if ($user->name != '' && isset($user->name)) {
      $file = file_save_data($image, 'public://' . $filename, FILE_EXISTS_RENAME);
      $user->field_photo = array(LANGUAGE_NONE => array('0' => (array)$file));
      user_save($user);
    }
  }
}