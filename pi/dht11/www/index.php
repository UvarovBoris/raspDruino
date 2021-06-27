<?php
    header('Content-Type: application/json');

    class Response {
        public $status;

        function __construct($status) {
            $this->status = $status;
        }
    }

    class DHTResponse extends Response {
        public $h;
        public $t;

        function __construct($h, $t) {
            parent::__construct("success");
            $this->h = $h;
            $this->t = $t;
        }
    }

    $dhtOutput;
    $dhtReturn;
    exec("./dht", $dhtOutput, $dhtReturn);

    if($dhtReturn == 0) {
        $rawData = $dhtOutput[0];
        $rawDataArray = explode('|', $rawData);

        $entriesArray = array();
        foreach($rawDataArray as &$rawDataEntry) {
            $entryDataArray = explode(':', $rawDataEntry);
            $entriesArray[$entryDataArray[0]] = $entryDataArray[1];
        }

        echo json_encode(new DHTResponse($entriesArray['h'], $entriesArray['t']));
   } else {
        echo json_encode(new Response("error"));
    }
?>
