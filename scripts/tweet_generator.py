import random

MIN_TIME = 1201977138
MAX_TIME = 1202510355

MIN_LAT = 39.5
MIN_LONG = 116.0

TIME_INTERVAL = MAX_TIME - MIN_TIME


tags = range(5)

tweets = []

# tweet form: time, lat, long, ht

# generate random tweets
for _up in range(16):
    for _right in range(16):
        # pick how many tweets for this box
        num_tweets = random.randint(25, 100)
        for _tweet in range(num_tweets):
            pass
            # pick time
            time = str(MIN_TIME + random.randint(0, TIME_INTERVAL))
            # pick precise location
            lat_p = str(random.randint(0, 999)).zfill(3)
            long_p = str(random.randint(0, 999)).zfill(3)
            lat_base = f"{39.5 + .05*_up:.2f}"
            long_base = f"{116 + .05*_up:.2f}"
            lat = lat_base + lat_p
            long = long_base + long_p
            # pick tag
            tag = str(random.choice(tags))
            # construct line item
            tweet = f"{time},{lat},{long},{tag}"
            tweets.append(tweet)


random.shuffle(tweets)
for t in tweets:
    print(t)
