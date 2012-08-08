<?php
// $Id$

/**
 * @file
 *
 */

function cg_tag_delete($nid, $natnid, $token) {
  if (drupal_valid_token($token, "delete $nid $natnid")) {
    $term = nat_get_term($natnid);
    db_query("DELETE FROM {term_node} WHERE nid = %d AND tid = %d", $nid, $term->tid);
  }
  drupal_json(TRUE);
  exit();
}

function cg_adjust_properties($nid, $action, $token) {
  $return = 'error';
  if (drupal_valid_token($token, "adjust-$nid-$action")) {
    $field = $action == 'sticky' ? 'sticky' : 'promote';
    db_query("UPDATE {node} SET $field = !$field WHERE nid = %d", $nid);
    $return = db_result(db_query("SELECT $action FROM {node} WHERE nid = %d", $nid)) ? 'Yes' : 'No';
  }
  drupal_json($return);
  exit();
}
