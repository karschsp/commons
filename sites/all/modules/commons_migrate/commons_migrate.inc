<?php

/**
 * To define a migration process from a set of source data to a particular
 * kind of Drupal object (for example, a specific node type), you define
 * a class derived from Migration. You must define a constructor to initialize
 * your migration object. By default, your class name will be the "machine name"
 * of the migration, by which you refer to it. Note that the machine name is
 * case-sensitive.
 *
 * In any serious migration project, you will find there are some options
 * which are common to the individual migrations you're implementing. You can
 * define an abstract intermediate class derived from Migration, then derive your
 * individual migrations from that, to share settings, utility functions, etc.
 */
abstract class CommonsMigration extends DynamicMigration {
  public function __construct() {
    // Always call the parent constructor first for basic setup
    parent::__construct();

    // With migrate_ui enabled, migration pages will indicate people involved in
    // the particular migration, with their role and contact info. We default the
    // list in the shared class; it can be overridden for specific migrations.
    /* $this->team = array(
      new MigrateTeamMember('Liz Taster', 'ltaster@example.com', t('Product Owner')),
      new MigrateTeamMember('Larry Brewer', 'lbrewer@example.com', t('Implementor')),
    ); */

    // Individual mappings in a migration can be linked to a ticket or issue
    // in an external tracking system. Define the URL pattern here in the shared
    // class with ':id:' representing the position of the issue number, then add
    // ->issueNumber(1234) to a mapping.
    //$this->issuePattern = 'http://drupal.org/node/:id:';
  }
}

class CommonsSponsorMigration extends BasicExampleMigration {
  public function __construct() {
    parent::__construct();
    $this->systemOfRecord = Migration::DESTINATION;
    $this->description = t('People Connections Sponsors');
    $this->map = new MigrateSQLMap($this->machineName,
        array('personid' => array(
              'type' => 'int',
              'not null' => TRUE,
              'description' => 'Person ID.'
              )
        ),
        MigrateDestinationUser::getKeySchema()
    );
    $query = "SELECT p.personid, p.firstname, p.lastname, p.firstname + '.' + p.lastname AS fullname, S.firstname + ' ' + S.lastname AS sponsor, p.email, p.title, p.extension, p.did, p.aboutme, p.location, p.site, p.type, p.remote, p.photo FROM person p join person s ON s.personid=p.supervisorid WHERE p.del IS NULL AND p.type IN ('Employee', 'Contingent Worker') ORDER BY p.lastname ASC;";
    $count_query = "SELECT count(personid) FROM person WHERE del IS NULL AND type IN ('Employee', 'Contingent Worker') AND PersonID IN (SELECT SupervisorID FROM Person WHERE type IN ('Employee', 'Contingent Worker') AND del IS NULL)";
    $fields = array(
      'personid' => t('Unique person id'),
      'firstname' => t('First Name'),
      'lastname' => t('Last Name'),
      'sponsor' => t('Sponsor'),
    );
    $configuration = array(
      'servername' => 'mssqlit80.vertexinc.com:10000',
      'username' => 'people',
      'password' => 'people',
      'database' => 'people',
    );

    $this->source = new MigrateSourceMSSQL($configuration, $query, $count_query, $fields);
    $this->destination = new MigrateDestinationUser();
    // One good way to organize your mappings is in three groups - mapped fields,
    // unmapped source fields, and unmapped destination fields

    // Mapped fields

    // This is a shortcut you can use when the source and destination field
    // names are identical (i.e., the email address field is named 'mail' in
    // both the source table and in Drupal).
    // $this->addSimpleMappings(array());

    // Our source table has two entries for 'alice', but we must have a unique
    // username in the Drupal 'users' table. dedupe() creates new, unique
    // destination values when the source field of that value already exists.
    // For example, if we're importing a user with name 'test' and a user
    // 'test' already exists in the target, we'll create a new user named
    // 'test_1'.
    // dedupe() takes the Drupal table and column for determining uniqueness.
    /* $this->addFieldMapping('name', 'name')
         ->dedupe('users', 'name');

    // The migrate module automatically converts date/time strings to UNIX timestamps.
    $this->addFieldMapping('created', 'posted');

    $this->addFieldMapping('pass', 'password'); */
    $this->addFieldMapping('uid', 'uid')->sourceMigration('CommonsUser');
    // Instead of mapping a source field to a destination field, you can
    // hardcode a default value. You can also use both together - if a default
    // value is provided in addition to a source field, the default value will
    // be applied to any rows where the source field is empty or NULL.

    // $this->addFieldMapping('field_migrate_example_gender', 'sex');

    // The source field has beer names separated by a pipe character ('|'). By
    // adding ->separator('|'), the migration will automatically break them out,
    // look up the node with each title, and assign the node reference to this
    // user.
    /* if (module_exists('node_reference')) {
      $this->addFieldMapping('field_sponsor', 'sponsor');
    } */

    // Unmapped source fields
    /* $this->addFieldMapping(NULL, 'nickname')
         ->issueGroup(t('DNM')); */

    // Unmapped destination fields

    // This is a shortcut you can use to mark several destination fields as DNM
    // at once
    $this->addUnmigratedDestinations(array('theme', 'signature', 'access', 'login',
      'timezone', 'language', 'picture', 'is_new', 'signature_format', 'role_names',
      'init', 'data'));

    // Oops, we made a typo - this should have been 'init'! If you have
    // migrate_ui enabled, look at the BeerUser info page - you'll see that it
    // displays a warning "int used as destination field in mapping but not in
    // list of destination fields", and also lists "1 unmapped" under Destination,
    // where it highlights "init" as unmapped.
    /* $this->addFieldMapping('int')
         ->issueGroup(t('DNM'));

    if (module_exists('path')) {
      $this->addFieldMapping('path')
           ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
             ->issueGroup(t('DNM'));
      }
    } */

  }

  public function prepare(stdClass $account, stdClass $row) {
    // Source dates are in ISO format.
    // Because the mappings above have been applied, $account->created contains
    // the date/time string now - we could also pass $row->posted here.
    /* $account->created = strtotime($account->created);
    $account->access = strtotime($account->access);
    $account->login = strtotime($account->login);
    $account->status = 1; */
    $account->field_sponsor[LANGUAGE_NONE][0]['uid'] = $this->findSponsorId($row->sponsor);
    // $account->field_sponsor[LANGUAGE_NONE][0]['uid'] = 9;

    // Gender data comes in as M/F, needs to be saved as Male=0/Female=1
    // TIP: Note that the Migration prepare method is called after all other
    // prepare handlers. Most notably, the field handlers have had their way
    // and created field arrays, so we have to save in the same format.
    /* switch ($row->sex) {
      case 'm':
      case 'M':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 0;
        break;
      case 'f':
      case 'F':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 1;
        break;
      default:
        unset($account->field_migrate_example_gender);
        break;
    }*/
  }
  public function findSponsorId($sponsor_name) {
    $sponsor = user_load_by_name($sponsor_name);
    if (isset($sponsor->uid)) {
      return $sponsor->uid;
    } else {
      return 0;
    }
  }


}

class CommonsTeamMigration extends BasicExampleMigration {
  public function __construct() {
    // The basic setup is similar to BeerTermMigraiton
    parent::__construct();
    $this->description = t('People Connections Teams');
    $this->map = new MigrateSQLMap($this->machineName,
        array('ID' => array(
              'type' => 'int',
              'not null' => TRUE,
              'description' => 'Team ID.'
              )
        ),
        MigrateDestinationNode::getKeySchema()
    );
    /* $query = db_select('person', 'p')
             ->fields('p', array('person', 'firstname', 'lastname', 'title', 'extension', 'did', 'aboutme', 'location', 'site', 'type', 'remote', )); */

    $query = "SELECT ID, Title, Acronym, EmailGroupName, Description FROM team_main";
    $count_query = "SELECT count(id) FROM team_main";

    $fields = array(
      'ID' => t('Team ID'),
      'Title' => t('Team Name'),
      'Acronym' => t('Acronym'),
      'EmailGroupName' => t('E-mail Group Name'),
      'Description' => t('Description'),
    );
    $configuration = array(
      'servername' => 'mssqlit80.vertexinc.com:10000',
      'username' => 'people',
      'password' => 'people',
      'database' => 'people',
    );

    $this->source = new MigrateSourceMSSQL($configuration, $query, $count_query, $fields);
    $this->destination = new MigrateDestinationNode('team');
    $this->addFieldMapping('group_group')->defaultValue(1);
    $this->addFieldMapping('og_roles_permissions')->defaultValue(1);
    $this->addFieldMapping('title', 'Title');
    $this->addFieldMapping('body', 'Description')->defaultValue('');
    $this->addFieldMapping('field_acronym', 'Acronym')->defaultValue('');
    $this->addFieldMapping('field_email_group_name', 'EmailGroupName')->defaultValue('');
    $this->addFieldMapping('uid')->defaultValue(1);
  }

  public function prepare(stdClass $node, stdClass $row) {
    if (stripos($row->Title, 'Strategy Team') > 0) {
      $team_type = "Strategy";
    } else if (stripos($row->Title, 'Key Focus Area') > 0 || stripos($row->Title, 'KFA') > 0) {
      $team_type = "Key Focus Area";
    } else if (stripos($row->Title, 'Guild')) {
      $team_type = "Guild";
    } else if (stripos($row->Title, 'Work Center Leadership') > 0) {
      $team_type = "Work Center Leadership";
    }
    if (isset($team_type)) {
      $node->field_team_type['und'][0]['value'] = $team_type;
    }
  }
}

class CommonsUserMigration extends BasicExampleMigration {
  public function __construct() {
    // The basic setup is similar to BeerTermMigraiton
    parent::__construct();
    $this->description = t('People Connections Users');
    $this->map = new MigrateSQLMap($this->machineName,
        array('personid' => array(
              'type' => 'int',
              'not null' => TRUE,
              'description' => 'Person ID.'
              )
        ),
        MigrateDestinationUser::getKeySchema()
    );
    /* $query = db_select('person', 'p')
             ->fields('p', array('person', 'firstname', 'lastname', 'title', 'extension', 'did', 'aboutme', 'location', 'site', 'type', 'remote', )); */
    $query = "SELECT p.personid, p.firstname, p.lastname, p.firstname + '.' + p.lastname AS fullname, S.firstname + ' ' + S.lastname AS sponsor, p.email, p.title, p.extension, p.did, p.aboutme, p.location, p.site, p.type, p.remote, p.photo FROM person p join person s ON s.personid=p.supervisorid WHERE p.del IS NULL AND p.type IN ('Employee', 'Contingent Worker') ORDER BY p.lastname ASC;";
    $count_query = "SELECT count(personid) FROM person WHERE del IS NULL AND type IN ('Employee', 'Contingent Worker')";
    $fields = array(
      'personid' => t('Unique person id'),
      'firstname' => t('First Name'),
      'lastname' => t('Last Name'),
      'email' => t('E-mail Address'),
      'title' => t('Title'),
      'extension' => t('Extension'),
      'did' => t('DID'),
      'aboutme' => t('About Me'),
      'location' => t('Location'),
      'site' => t('Site'),
      'type' => t('Type'),
      'remote' => t('Remote'),
      'sponsor' => t('Sponsor'),
      // 'photo' => t('Photo'),
    );
    $configuration = array(
      'servername' => 'mssqlit80.vertexinc.com:10000',
      'username' => 'people',
      'password' => 'people',
      'database' => 'people',
    );

    $this->source = new MigrateSourceMSSQL($configuration, $query, $count_query, $fields);
    $this->destination = new MigrateDestinationUser();

    // One good way to organize your mappings is in three groups - mapped fields,
    // unmapped source fields, and unmapped destination fields

    // Mapped fields

    // This is a shortcut you can use when the source and destination field
    // names are identical (i.e., the email address field is named 'mail' in
    // both the source table and in Drupal).
    // $this->addSimpleMappings(array());

    // Our source table has two entries for 'alice', but we must have a unique
    // username in the Drupal 'users' table. dedupe() creates new, unique
    // destination values when the source field of that value already exists.
    // For example, if we're importing a user with name 'test' and a user
    // 'test' already exists in the target, we'll create a new user named
    // 'test_1'.
    // dedupe() takes the Drupal table and column for determining uniqueness.
    /* $this->addFieldMapping('name', 'name')
         ->dedupe('users', 'name');

    // The migrate module automatically converts date/time strings to UNIX timestamps.
    $this->addFieldMapping('created', 'posted');

    $this->addFieldMapping('pass', 'password'); */
    $this->addFieldMapping('field_name_first', 'firstname');
    $this->addFieldMapping('field_name_last', 'lastname');
    $this->addFieldMapping('name', 'fullname');
    $this->addFieldMapping('field_bio', 'aboutme');
    $this->addFieldMapping('mail', 'email');
    $this->addFieldMapping('field_extension', 'extension');
    $this->addFieldMapping('field_site', 'site');
    $this->addFieldMapping('field_title', 'title');
    $this->addFieldMapping('field_did', 'did');
    $this->addFieldMapping('field_location', 'location');
    $this->addFieldMapping('field_remote', 'remote');
    $this->addFieldMapping('field_sponsor_name', 'sponsor');
    /* $this->addFieldMapping('field_sponsor', 'sponsor')
         ->sourceMigration('CommonsUser'); */
    /* $this->addFieldMapping('field_photo', 'photo')
         ->description('An image blob in the DB')
         ->arguments(array(
              'file_function' => 'file_blob',
              // Alternatively, specify a column here for dynamic file name.
              'source_path' => 'photo' . time() . '.jpg',
           )); */


    // Instead of mapping a source field to a destination field, you can
    // hardcode a default value. You can also use both together - if a default
    // value is provided in addition to a source field, the default value will
    // be applied to any rows where the source field is empty or NULL.
    $this->addFieldMapping('roles')
         ->defaultValue(DRUPAL_AUTHENTICATED_RID);

    // $this->addFieldMapping('field_migrate_example_gender', 'sex');

    // The source field has beer names separated by a pipe character ('|'). By
    // adding ->separator('|'), the migration will automatically break them out,
    // look up the node with each title, and assign the node reference to this
    // user.
    /* if (module_exists('user_reference')) {
      $this->addFieldMapping('field_sponsor_uid', 'sponsor')
        ->sourceMigration(array('CommonsUser'));
    } */

    // Unmapped source fields
    /* $this->addFieldMapping(NULL, 'nickname')
         ->issueGroup(t('DNM')); */

    // Unmapped destination fields

    // This is a shortcut you can use to mark several destination fields as DNM
    // at once
    $this->addUnmigratedDestinations(array('theme', 'signature', 'access', 'login',
      'timezone', 'language', 'picture', 'is_new', 'signature_format', 'role_names',
      'init', 'data'));

    // Oops, we made a typo - this should have been 'init'! If you have
    // migrate_ui enabled, look at the BeerUser info page - you'll see that it
    // displays a warning "int used as destination field in mapping but not in
    // list of destination fields", and also lists "1 unmapped" under Destination,
    // where it highlights "init" as unmapped.
    /* $this->addFieldMapping('int')
         ->issueGroup(t('DNM'));

    if (module_exists('path')) {
      $this->addFieldMapping('path')
           ->issueGroup(t('DNM'));
      if (module_exists('pathauto')) {
        $this->addFieldMapping('pathauto')
             ->issueGroup(t('DNM'));
      }
    } */
  }

  /* protected function createStub(Migration $migration, array $source_id) {
    drupal_set_message('SOURCE ID: ' . $source_id[0]);
    $user = user_load_by_name($source_id[0]);
    if (isset($user->uid)) {
      return array($user->uid);
    } else {
      $user = new stdClass();
      $user->name = t('@id', array('@id' => $source_id[0]));
      $user->status = 1;
      user_save($user);
      if (isset($user->uid)) {
        return array($user->uid);
      } else {
        return FALSE;
      }
    }
  } */
  /* public function prepareRow($row) {
    // print_r($row);
    $sponsor = user_load_by_name($row->sponsor);
    // print_r($sponsor);
    if (isset($sponsor->uid)) {
      print($sponsor->uid) . "\n";
      $row->field_sponsor_uid = $sponsor->uid;
    } else {
      $sponsor = new stdClass();
      $sponsor->name = $row->sponsor;
      $sponsor->status = 1;
      user_save($sponsor);
      print($sponsor->uid);
      // $row->sponsor = $sponsor->uid;
      $row->field_sponsor_uid = $sponsor->uid;
    }

  } */
  public function prepare(stdClass $account, stdClass $row) {
    // Source dates are in ISO format.
    // Because the mappings above have been applied, $account->created contains
    // the date/time string now - we could also pass $row->posted here.
    $account->created = strtotime($account->created);
    $account->access = strtotime($account->access);
    $account->login = strtotime($account->login);
    $account->status = 1;
    // $account->field_sponsor[LANGUAGE_NONE][0]['uid'] = $this->findSponsorId($row->sponsor);
    // $account->field_sponsor[LANGUAGE_NONE][0]['uid'] = 9;

    // Gender data comes in as M/F, needs to be saved as Male=0/Female=1
    // TIP: Note that the Migration prepare method is called after all other
    // prepare handlers. Most notably, the field handlers have had their way
    // and created field arrays, so we have to save in the same format.
    /* switch ($row->sex) {
      case 'm':
      case 'M':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 0;
        break;
      case 'f':
      case 'F':
        $account->field_migrate_example_gender[LANGUAGE_NONE][0]['value'] = 1;
        break;
      default:
        unset($account->field_migrate_example_gender);
        break;
    }*/
  }
  /* public function findSponsorId($sponsor_name) {
    $sponsor = user_load_by_name($sponsor_name);
    if (isset($sponsor->uid)) {
      return $sponsor->uid;
    } else {
      return 0;
    }
  } */
}

class CommonsOGMigration extends CommonsMigration {
  public function __construct() {
    parent::__construct();
    $this->description = t('Migrate Team Memberships');

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'ID' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'ID of the group',
        ),
        'personid' => array(
          'type' => 'int',
          'not null' => TRUE,
          'description' => 'ID of the user',
        ),
      ),
      MigrateDestinationOGMembership::getKeySchema()
    );

    $query = "SELECT tp.ID, p.personid, p.firstname, p.lastname, p.email, t.Title FROM Team_To_Person tp INNER JOIN person p ON p.personid = tp.PersonID INNER JOIN Team_Main t ON t.ID = tp.TeamID WHERE t.Title LIKE '%Strategy Team%'";
    $count_query = "SELECT count(tp.ID) FROM Team_To_Person tp INNER JOIN person p ON p.personid = tp.PersonID INNER JOIN Team_Main t ON t.ID = tp.TeamID WHERE t.Title LIKE '%Strategy Team%'";
    $fields = array(
      'ID' => t('tp ID'),
      'personid' => t('Unique person id'),
      'firstname' => t('First Name'),
      'lastname' => t('Last Name'),
      'email' => t('E-mail Address'),
      'title' => t('Team Name'),

    );
    $configuration = array(
      'servername' => 'mssqlit80.vertexinc.com:10000',
      'username' => 'people',
      'password' => 'people',
      'database' => 'people',
    );

    $this->source = new MigrateSourceMSSQL($configuration, $query, $count_query, $fields);
    $this->destination = new MigrateDestinationOGMembership();

    $this->addFieldMapping('group_type')->defaultValue('node');
    $this->addFieldMapping('bundle')->defaultValue('strategy_team');
    $this->addFieldMapping('type')->defaultValue('og_membership_type_default');
    $this->addFieldMapping('gid', 'Title');
    //->sourceMigration('CommonsOGMigration');

    $this->addFieldMapping('entity_type')->defaultValue('user');
    $this->addFieldMapping('etid', 'personid');
        // ->sourceMigration('CommonsUserMigration');

    $this->addFieldMapping('state')->defaultValue(OG_STATE_ACTIVE);
    $this->addFieldMapping('is_admin')->defaultValue(0);
  }

  public function prepareRow(stdClass $row) {
    print_r($row);
    $team_name = $row->Title;
    print $team_name . "\n";
    $query = db_select('node', 'n');
    $query->condition('n.title', $team_name, '=');
    $query->fields('n', array('nid'));
    $result = $query->execute();
    foreach ($result as $record) {
      print($record->nid) . "\n";
      $row->gid = $record->nid;
    }

    /* $sql = "SELECT nid FROM {node} WHERE title='%s'";
    $nid = db_query(db_result($sql, $team_name));
    $row->gid = $nid; */

    $name = $row->firstname . ' ' . $row->lastname;
    /* $sql = "SELECT uid FROM {user} WHERE name='%s'";
    $uid = db_query(db_result($sql, $name));
    $row->uid = $uid; */

    $query = db_select('users', 'u');
    $query->condition('u.name', $name, '=');
    $query->fields('u', array('uid'));
    $result = $query->execute();
    foreach ($result as $record) {
      $row->uid = $record->uid;
    }
  }
}