#include "jpeg_decoder.h"

MarkerType marker_type(uint16_t marker)
{
    switch (marker) {
    case 0xffd8: return SOI;
    case 0xffd9: return EOI;
    case 0xffe0: return APP0;
    case 0xffdb: return DQT;
    case 0xffc0: return SOF0;
    case 0xffc4: return DHT;
    case 0xffda: return SOS;
    default: return UNKNOWN;
    }
}

MarkerSegment *read_segment(FILE *fp)
{
    MarkerSegment *seg = (MarkerSegment*)malloc(sizeof(MarkerSegment));
    uint16_t marker;
    fread(&marker, sizeof(uint16_t), 1, fp);
    marker = be16toh(marker);
    seg->type = marker_type(marker);
    switch (seg->type) {
    case SOI:
    case EOI:
        seg->len = 0;
        seg->data.ptr = NULL;
        break;
    case UNKNOWN:
        printf("unknown marker %x\n", marker);
        free(seg);
        return NULL;
    default:
        fread(&seg->len, sizeof(uint16_t), 1, fp);
        seg->len = be16toh(seg->len);
        seg->data.ptr = malloc(seg->len - 2);
        fread(seg->data.ptr, seg->len - 2, 1, fp);
    }
    printf("read segment, marker = %x, len = %d\n", marker, seg->len);
    return seg;
}
