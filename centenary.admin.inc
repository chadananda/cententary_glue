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
