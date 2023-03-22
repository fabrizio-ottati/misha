# Misha

A package with minimum dependencies to decode data produced by event cameras.

## Misha Mahowald

![Misha Mahowald](/docs/images/misha.jpg) 

*Misha Mahowald (circa 1992) in the Carverland laboratory at Caltech, testing her stereo correspondence chip. Photo credit: Rodney Douglas. [Source](https://lenzgregor.com/posts/event-cameras/).*


Towards the end of the 80s, a scientist at Caltech named **Carver Mead** spawned the field of Neuromorphic Engineering, when one of his students called **Misha Mahowald** developed a **new stereo vision system**. Taking inspiration from the **human visual system**, she built what would become the first **silicon retina** in the early '90s. It was based on the same principle of center surround receptive fields in the human retina, that emit spikes independently of each other depending on the contrast pattern observed.

Although Misha drafted the beginning of a new imaging sensor, it did not provide a practical implementation at first. In response, the neuromorphic community simplified the problem by dropping the principle of center-surround pixels. Instead of encoding spatial contrast across multiple pixels which needed sophisticated circuits, the problem could be alleviated by realising a circuit that could encode temporal contrast for single pixels. That way, pixels could still operate individually as processing units just as receptive fields in the retina do and report any deviations in illuminance over time. It would take until 2001 when Tetsuya Yagi at Osaka University and Tobi Delbrück at UZH/ETH in 2008 publish about the first refined temporal contrast sensors, the event cameras as they are known today.

## Credits

* [Gregor Lenz](https://lenzgregor.com), for his [blog post](https://lenzgregor.com/posts/event-cameras/) on Misha Mahowald's history, copied and pasted here.
