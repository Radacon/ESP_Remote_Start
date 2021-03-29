# ESP_Remote_Start

<h2>Motivation & Abstract</h2>
<p> Chevy wanted $15/month to be able to start/lock/unlock/find my wife's new car with an app. This is silly but pretty useful. Is it worth $15/month? No, no it's not.</p>
<p> What I have done is developed a website that is hosted on an ESP8266. Through this website a user can start, stop, lock, and unlock their vehicle. The website is also capable of scheduling remote-start times. This is great in the winter when you don't think about starting your vehicle until you walk out. Snow in the forecast tonight? Schedule a remote start 10 minutes before you leave from work to help the scraping process. The extra idle-time is nice for getting the lubricant's moving again after sitting all night.</p>

<h2>Warnings</h2>

<p> First thing's first. Please don't even consider building one of these if you park your cars in a garage. A single error in the code could kill you and the code is not without it's flaws! Second, I implement OTA updating, this is in no way locked down to prevent some rando with a laptop in your neighborhood from pushing nefarious code onto the device.</p> 
<p> With that said, if you park your vehicles outside, and are willing to accept the risk of someone remote starting it/unlocking it to steal your stuff then by all means continue on. </p>
