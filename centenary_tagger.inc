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