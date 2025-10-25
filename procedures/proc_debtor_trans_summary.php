<?php

$mysqli = new mysqli("localhost", "root", "j311214321", "uxira");
if ($mysqli->connect_errno) {
    printf("Falló la conexión: %s\n", $mysqli->connect_error);
    exit();
}

$sql = "DELETE FROM 0_debtor_trans_summary";

if ($mysqli->query($sql) === TRUE) {
   echo "1 - 0_debtor_trans_summary table data deleted successfully";
   printf(" - Affected rows (DELETE): %d\n", $mysqli->affected_rows);
}
else {
    echo "Error: " . $sql . "\n" . $mysqli->error;
}

$sql = "INSERT INTO 0_debtor_trans_summary 
(trans_no,account_associated,ov_amount,particular_amount,insur_comp_amount)
SELECT trans_no,account_associated,sum(ov_amount) as ov_amount,0,0 
FROM 0_debtor_trans dt where type = 10 and ov_amount >= 1 and serial != '0' and
no_control != '' and  account_associated != 0 group by account_associated";

if ($mysqli->query($sql) === TRUE) {
   echo "\n2 - New records created successfully";
   printf(" - Affected rows (INSERT): %d\n", $mysqli->affected_rows);
}
else {
    echo "Error: " . $sql . "\n" . $mysqli->error;
}

$sql = "update 0_debtor_trans_summary as t1,
(SELECT sum(ov_amount) as ov_amount, account_associated
FROM 0_debtor_trans dt where type in (2,12) and ov_amount > 0 and account_associated != 0 
group by account_associated ) as t2
set t1.particular_amount = 0
where t1.account_associated = t2.account_associated;";

if ($mysqli->query($sql) === TRUE) {
   echo "\n3 - Update 0_debtor_trans_summary particular_amount = 0";
   printf(" - Affected rows (UPDATE): %d\n", $mysqli->affected_rows);
}
else {
    echo "Error: " . $sql . "\n" . $mysqli->error;
}

$sql = "update 0_debtor_trans_summary as t1,
(SELECT sum(ov_amount) as ov_amount, account_associated
FROM 0_debtor_trans dt where type in (2,12) and ov_amount > 0 and account_associated != 0 
group by account_associated ) as t2
set t1.particular_amount = t1.particular_amount + t2.ov_amount
where t1.account_associated = t2.account_associated;";

if ($mysqli->query($sql) === TRUE) {
   echo "\n4 - Update 0_debtor_trans_summary particular_amount = t1.particular_amount + t2.ov_amount";
   printf(" - Affected rows (UPDATE): %d\n", $mysqli->affected_rows);
}
else {
    echo "Error: " . $sql . "\n" . $mysqli->error;
}

$sql = "update 0_debtor_trans_summary as t1,
(SELECT dt.account_associated, sum(dtr.ov_amount + std.dscto_amount + std.tax_amount) as amount
 FROM 0_debtor_trans dt,
            0_debtor_trans_respons_paym dtr, 0_bank_trans bt, 0_settlement_trans_detail std
where dt.type = 10 and dtr.account_associated = dt.account_associated
      and dtr.type = 80
      and bt.type = 2 and bt.trans_no = dtr.bank_tran and bt.person_type_id =
		 7 and dt.ov_amount > 0
      and std.payment_rel = dtr.trans_no and std.type = 78
      group by dt.account_associated) as t2
set t1.insur_comp_amount = t2.amount
where t1.account_associated = t2.account_associated;";

if ($mysqli->query($sql) === TRUE) {
   echo "\n5 - Update 0_debtor_trans_summary t1.insur_comp_amount = t2.amount";
   printf(" - Affected rows (UPDATE): %d\n", $mysqli->affected_rows);
}
else {
    echo "Error: " . $sql . "\n" . $mysqli->error;
}

$mysqli->close();
        
?>

