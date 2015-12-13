<?php

$url = 'http://concordia.vm.wmi.amu.edu.pl:8800';
$data = array("operation" => $_POST["operation"],"tmId" => intval($_POST["tmId"]),"pattern" => $_POST["pattern"]);

// use key 'http' even if you send the request to https://...
$options = array(
    'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => json_encode($data),
    ),
);
$context  = stream_context_create($options);
$result = file_get_contents($url, false, $context);

print $result;

?>
