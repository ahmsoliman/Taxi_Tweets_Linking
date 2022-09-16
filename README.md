## Taxi Tweets Linking Project

### Overview

Given two datasets (shown below) namely, 200_taxi_database.csv and tweet_database.csv, link records based on spatio-temporal values within a specified space and time resolutions.

### Data preparation

This project uses a subset of the T-drive Taxi Trajectories Dataset

- [Download link](http://onedrive.live.com/?authkey=%21ADgmvTgfqs4hn4Q&id=CF159105855090C5%211438&cid=CF159105855090C5)

### Data format:

200_taxi_database.csv columns:

`taxi_id`, `datetime (unix)`, `longitude`, `latitude`

Here is a sample:

```
1,1201984568,39.92123,116.51172
1,1201985168,39.93883,116.51135
1,1201985768,39.91034,116.51627
1,1201986368,39.91248,116.47186
1,1201986968,39.92498,116.47217
1,1201987568,39.90718,116.47179
```

tweet_database.csv columns:

`datetime (unix)`, `longitude`, `latitude`, `hashtag_id`

First few lines from the tweet_database.csv:

```
1202054629,40.05692,116.55917,3
1202136521,39.75669,116.25145,4
1202470194,39.90391,116.40569,0
1202265210,39.50937,116.00777,4
1202243324,39.70250,116.20126,1
```

Where the `hashtag_id = [0|1|2|3|4]`.

### Desired output:

A sample output file is shown below:

`taxi_id`, `datetime (unix)`, `longitude`, `latitude`, `ht0`, `ht1`, `ht2`, `ht3`, `ht4`

Where the values in the `htx` columns is `[0|100]` depending on whether the hashtag is associated with the current record.

### Steps:

  1. Sort the T-drive dataset by date time then by longitude then by latitude. (append `type` = 0 as a last attribute)
  2. Sort the Tweets dataset by date time then by longitude then by latitude. (append `type` = 1 as a last attribute)
  3. Merge the two sorted lists obtained from steps 1 & 2 above.
  4. Scan through the merged list obtained from step 3 and do the following:<br>
     &nbsp;For each two consecutive lines/records do the following:<br>
     &nbsp;&nbsp;if both lines belong to different datasets (i.e. different types) AND the difference in datetime is less than the specified time resolution AND the difference in distance (Euclidean distance as defined by lat & long information) is less than the specified space resolution.
     &nbsp;&nbsp;Then Associate the `tweet id` with the `taxi id`.   

### How to run:

```shell
./ttl 200_taxi_database.csv tweet_database.csv taxi_tweet_database.csv
```

### TODO:

 - Add space and time resolutions as attributes in one or both of the tables. Since precisions of data values vary based on employed acquisition techniques (e.g. WiFi, GPS, Assisted-GPS, ISP gateway information ...etc).



