""" A dataclass for keeping track of all the properties of a network flow. """

from dataclasses import dataclass
from collections.abc import Callable

@dataclass
class Flow:
    """ A dataclass for keeping track of all the properties of a network flow. """
    fid: int  # flow id
    # fid: Sketches.Sketches.TUPLES  # flow id
    src: str  # source element
    dst: str  # destination element
    fg_id: int = 0 #flow group id
    size: int = None  # flow size in bytes
    start_time: float = None
    finish_time: float = None
    arrival_dist: Callable = None  # packet arrival distribution
    size_dist: Callable = None  # packet size distribution
    pkt_gen: object = None
    pkt_sink: object = None
    path: tuple = None
    is_big: int = 0

    def __repr__(self) -> str:
        return f"Flow {self.fid} on {self.path}"
