<?php
// $Id$

/**
 * @file
 *
 */

function cg_settings_form() {
  $form['facebook'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['facebook']['cg_fb_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#default_value' => variable_get('cg_fb_app_id', ''),
  );
  $form['facebook']['cg_fb_app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#default_value' => variable_get('cg_fb_app_secret', ''),
  );
//  $form['facebook']['test_facebook'] = array(
//    '#type' => 'submit',
//    '#value' => 'Test Facebook',
//  );

  $form['twitter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['twitter']['cg_twitter_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Key'),
    '#default_value' => variable_get('cg_twitter_key', ''),
  );
  $form['twitter']['cg_twitter_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Secret'),
    '#default_value' => variable_get('cg_twitter_secret', ''),
  );
//  $form['twitter']['test_twitter'] = array(
//    '#type' => 'submit',
//    '#value' => 'Test Twitter',
//  );

  $form['#submit'] = array('cg_settings_form_submit'); 

  return system_settings_form($form);
}

function cg_settings_form_submit(&$form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Test Facebook') {
  }
  elseif ($form_state['clicked_button']['#value'] == 'Test Twitter') {
  }
}

function cg_daily_archive($date = FALSE, $all = FALSE) {

  // CONTENT
  if (!$date = cg_daily_archive_date($date)) {
    drupal_goto('daily-archive/1912-' . format_date(time(), 'custom', 'm'));
  }
  drupal_set_html_head("<link rel='canonical' href='http://centenary.bahai.us/daily-archive/$date' />");
  $rows = cg_daily_archive_data(cg_daily_archive_date($date), $all);
  drupal_set_title("Daily Centenarian Archive : " . format_date(strtotime($date), 'custom', 'F Y'));
  if (!is_array($rows)) {
    $output = t("<p>There are no pages available for this month.</p>");
  }
  else {
    foreach ($rows as $r) {
      $url_date = str_replace('T00:00:00', '', $r->date);
      $display_date = format_date(strtotime($r->date), 'custom', 'D, F j, Y');
      $title = check_plain(cg_fix_bahai_words($r->title));
      $img = imagecache_create_url('teaser', $r->img);
      $link = "/daily/$url_date";
      $more = format_plural($r->count, '1 article', '@count articles');
      $text = <<<EOF
<div class="view-id-daily view-display-id-block_1">
  <div class="view-header">
    <p><a href="$link" class="daily-centenarian-link"><span class="daily-headline">$display_date</span></a></p>
  </div>
  <div class="view-content">
    <div class="views-field-field-main-image-fid">
      <a href="$link"><img width="55" height="55" src="$img" class="imagecache imagecache-teaser imagecache-default imagecache-teaser_default"></a>
    </div>
    <div class="views-field-title">
      <a href="$link">$title</a>
    </div>
    <div class="views-field-nothing">
      <div class="more-link"><a href="$link">$more</a></div>
    </div>
  </div>
</div>
EOF;
      $output = $text . $output;
    }
  }

  // NAVIGATION
  $thisdate = format_date(time(), 'custom', 'y-m-d');
  $notall = ($all && user_access('administer nodes')) ? '' : "AND d.field_historical_date_value <= '19$thisdate'";
  if ($lastdate = db_result(db_query("SELECT d.field_historical_date_value
    FROM {node} n LEFT JOIN {content_field_historical_date} d USING (vid) WHERE
    n.status AND n.type IN ('clip', 'talk', 'vignette') AND
    d.field_historical_date_value < '%s' AND d.field_historical_date_value >= '1911-01-01T00:00:00' $notall
    ORDER BY d.field_historical_date_value DESC LIMIT 0,1", "$date-01"))) {
    $links = '<div class="daily-archive-link prev">' .
      l('<< ' . format_date(strtotime($lastdate), 'custom', 'F Y'),
        'daily-archive/' . substr($lastdate, 0, 7) . ($notall ? '' : '/all'))
        . '</div>';
  }
  if ($nextdate = db_result(db_query("SELECT d.field_historical_date_value
    FROM {node} n LEFT JOIN {content_field_historical_date} d USING (vid) WHERE
    n.status AND n.type IN ('clip', 'talk', 'vignette') AND
    d.field_historical_date_value >= '%s' AND d.field_historical_date_value <= '1913-12-31T00:00:00' $notall
    ORDER BY d.field_historical_date_value ASC LIMIT 0,1", format_date(strtotime("$date +1 month"), 'custom', 'Y-m')))) {
    $links .= '<div class="daily-archive-link next">' .
      l(format_date(strtotime($nextdate), 'custom', 'F Y') . ' >>',
        'daily-archive/' . substr($nextdate, 0, 7) . ($notall ? '' : '/all'))
        . '</div>';
  }
  $links = "<div class='daily-archive-nav-links'>$links</div>";
  $nav .= '<div class="daily-archive-month-links clearfix">';
  $current_month = format_date(time(), 'custom', 'y-m');
  for ($t = 1; $t <= 12; $t++) {
    $m = str_pad($t, 2, '0', STR_PAD_LEFT);
    if (!$notall || ("12-$m" <= $current_month)) {
      $nav .= l(format_date(strtotime("1912-$m"), 'custom', 'M'), "daily-archive/1912-$m" . ($notall ? '' : '/all'));
    }
    else {
      break;
    }
  }
  $nav .= '</div>';
  return $links . $nav . $output;
}

function cg_daily_archive_rss() {
  $q = cg_daily_archive_query('1912', FALSE);

}

function cg_daily_archive_date($date = FALSE) {
  $matches = array();
  if (!preg_match('|\d\d\d\d-\d\d|', $date, $matches) || !$date = $matches[0]) {
    return FALSE;
  }
  return $date;
}

function cg_daily_archive_data($month, $all = FALSE) {

  if (!preg_match('|^191[1-3]|', $month)) {
    return FALSE;
  }

  // Text for the final query
  $query = "SELECT n.title,
    d.field_historical_date_value AS date, f.filepath AS img
    FROM {node} n LEFT JOIN {content_field_historical_date} d USING (vid)
    LEFT JOIN {content_field_main_image} i USING (vid)
    LEFT JOIN {files} f ON (i.field_main_image_fid = f.fid)
    WHERE n.status AND n.type IN ('clip', 'talk', 'vignette') AND
    d.field_historical_date_value = '%s'
    ORDER BY n.sticky DESC, n.type ASC, n.moderate DESC LIMIT 0,1";

  // build the date query
  $where = ("n.status AND n.type IN ('clip', 'talk', 'vignette') AND
    d.field_historical_date_value BETWEEN '%s' AND '%s'");
  $args = array("$month-01", "$month-31");
  if (!$all) {
    $where .= " AND d.field_historical_date_value <= '%s'";
    $args[] = '1912-' . format_date(time(), 'custom', 'm-d') . 'T00:00:00';
  }
  $datequery = db_query("SELECT COUNT(n.nid) AS count,
    d.field_historical_date_value AS date FROM {node} n
    LEFT JOIN {content_field_historical_date} d USING (vid)
    WHERE $where GROUP BY d.field_historical_date_value
    ORDER BY d.field_historical_date_value DESC LIMIT 0,31", $args);

  // fetch the real data
  while($date = db_fetch_object($datequery)) {
    $data[$date->date] = db_fetch_object(db_query($query, $date->date));
    $data[$date->date]->count = $date->count;
  }
  return $data;
}
