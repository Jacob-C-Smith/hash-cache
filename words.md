## Hash Cache > Word Frequencies
The lorem_ipsum.txt file is used to test the cache data structure. The file contains 1,000,000 words, 169 of which are unique. See [lorem_ipsum.zip](lorem_ipsum.zip)

Below is the frequency of each word in a tabular representation. 

| Count        | Word  |
|--------------|-------|
| sed          | 23754 |
| in           | 20099 |
| amet         | 19010 |
| sit          | 18981 |
| id           | 16018 |
| ut           | 15754 |
| eget         | 15251 |
| et           | 14610 |
| vitae        | 14526 |
| nunc         | 14388 |
| at           | 13514 |
| enim         | 12569 |
| pellentesque | 11968 |
| egestas      | 11873 |
| eu           | 11818 |
| ac           | 11136 |
| quis         | 11125 |
| diam         | 10910 |
| viverra      | 10902 |
| nulla        | 10414 |
| mauris       | 10217 |
| tellus       | 10202 |
| massa        | 10200 |
| arcu         | 10153 |
| non          | 10149 |
| aliquam      | 10110 |
| faucibus     | 9600  |
| tincidunt    | 9452  |
| morbi        | 9373  |
| risus        | 9225  |
| volutpat     | 8636  |
| urna         | 8576  |
| a            | 8570  |
| vel          | 8455  |
| neque        | 8449  |
| tortor       | 8442  |
| purus        | 8438  |
| turpis       | 8410  |
| nibh         | 8360  |
| adipiscing   | 8325  |
| odio         | 7751  |
| elementum    | 7737  |
| quam         | 7698  |
| lectus       | 7694  |
| aliquet      | 7609  |
| cursus       | 7594  |
| ultrices     | 7567  |
| scelerisque  | 7485  |
| nisl         | 7475  |
| lacus        | 6974  |
| elit         | 6946  |
| mi           | 6922  |
| orci         | 6885  |
| feugiat      | 6846  |
| tristique    | 6828  |
| pharetra     | 6786  |
| nec          | 6767  |
| est          | 6758  |
| mattis       | 6723  |
| velit        | 6706  |
| gravida      | 6383  |
| ipsum        | 6184  |
| donec        | 6086  |
| malesuada    | 6047  |
| vulputate    | 5987  |
| leo          | 5970  |
| dui          | 5957  |
| cras         | 5954  |
| ornare       | 5933  |
| pretium      | 5921  |
| commodo      | 5899  |
| ullamcorper  | 5884  |
| sagittis     | 5876  |
| pulvinar     | 5803  |
| consectetur  | 5668  |
| dolor        | 5399  |
| lorem        | 5230  |
| suspendisse  | 5189  |
| integer      | 5159  |
| bibendum     | 5132  |
| fermentum    | 5121  |
| ultricies    | 5119  |
| dignissim    | 5110  |
| blandit      | 5088  |
| consequat    | 5084  |
| facilisis    | 5073  |
| nisi         | 5050  |
| augue        | 5043  |
| semper       | 5026  |
| duis         | 5013  |
| magna        | 4977  |
| vestibulum   | 4937  |
| proin        | 4652  |
| tempus       | 4359  |
| posuere      | 4357  |
| sapien       | 4325  |
| felis        | 4305  |
| interdum     | 4291  |
| maecenas     | 4287  |
| rhoncus      | 4286  |
| tempor       | 4285  |
| porttitor    | 4255  |
| convallis    | 4234  |
| aenean       | 4234  |
| imperdiet    | 4231  |
| etiam        | 4202  |
| auctor       | 4201  |
| euismod      | 4189  |
| sem          | 4188  |
| venenatis    | 4154  |
| dictum       | 4154  |
| varius       | 4130  |
| libero       | 4107  |
| congue       | 3511  |
| facilisi     | 3498  |
| lobortis     | 3491  |
| sollicitudin | 3486  |
| molestie     | 3470  |
| nullam       | 3454  |
| iaculis      | 3443  |
| laoreet      | 3430  |
| accumsan     | 3411  |
| condimentum  | 3404  |
| quisque      | 3395  |
| fringilla    | 3393  |
| erat         | 3386  |
| justo        | 3288  |
| placerat     | 3195  |
| porta        | 2591  |
| praesent     | 2586  |
| eleifend     | 2584  |
| fames        | 2575  |
| habitant     | 2568  |
| netus        | 2557  |
| senectus     | 2553  |
| sodales      | 2536  |
| phasellus    | 2513  |
| metus        | 2470  |
| nam          | 2459  |
| hendrerit    | 2027  |
| hac          | 1757  |
| curabitur    | 1756  |
| habitasse    | 1747  |
| dictumst     | 1744  |
| platea       | 1736  |
| lacinia      | 1723  |
| vivamus      | 1708  |
| rutrum       | 1703  |
| suscipit     | 1676  |
| eros         | 1674  |
| fusce        | 1672  |
| mollis       | 1670  |
| luctus       | 1661  |
| ante         | 1654  |
| parturient   | 927   |
| mus          | 924   |
| magnis       | 909   |
| dis          | 909   |
| montes       | 908   |
| ridiculus    | 902   |
| nascetur     | 902   |
| vehicula     | 894   |
| penatibus    | 894   |
| natoque      | 888   |
| potenti      | 885   |
| dapibus      | 874   |
| cum          | 871   |
| sociis       | 868   |
| ligula       | 849   |